#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIAsyncExecution"

#include "AIFactory.hpp"

#include "IBehaviorTree.hpp"
#include "ISelector.hpp"

#include "Mock_AIAction.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/assign.hpp>

#include <vector>
#include <ostream>


namespace aw {
namespace core {
namespace ai {

std::ostream& operator<< (std::ostream& stream, const BehaviorTreeState& state) {

    stream << static_cast<int>(state);
    return stream;
}

namespace testing {

// Layout of the tree used throughout the test:
//            selectorA
//            /       \
// failingAction     selectorB
//                   /       \
//        failingAction     succeedingAction



struct AIAsyncFixture {

    AIAsyncFixture() {

        BOOST_CHECK_NO_THROW(selectorA = aiFactory.createSelector());
        BOOST_CHECK_NO_THROW(selectorB = aiFactory.createSelector());
        BOOST_REQUIRE(selectorA);
        BOOST_REQUIRE(selectorB);

        BOOST_CHECK_NO_THROW(failingAction = aiFactory.createAction([](IBlackboardPtr /*blackboard*/){return TaskResult::TASK_RESULT_FAILED;}));
        BOOST_CHECK_NO_THROW(succeedingAction = aiFactory.createAction([](IBlackboardPtr /*blackboard*/){return TaskResult::TASK_RESULT_PASSED;}));
        BOOST_REQUIRE(failingAction);
        BOOST_REQUIRE(succeedingAction);

        BOOST_CHECK_NO_THROW(selectorA->AddTask(failingAction));
        BOOST_CHECK_NO_THROW(selectorA->AddTask(selectorB));
        BOOST_CHECK_NO_THROW(selectorB->AddTask(failingAction));
        BOOST_CHECK_NO_THROW(selectorB->AddTask(succeedingAction));

        BOOST_CHECK_EQUAL(selectorA->GetNumTasks(), 2);
        BOOST_CHECK_EQUAL(selectorB->GetNumTasks(), 2);

        BOOST_CHECK_NO_THROW(syncTree = aiFactory.createBehaviorTree());
        BOOST_CHECK_NO_THROW(asyncTree = aiFactory.createBehaviorTree());

        BOOST_CHECK_NO_THROW(syncTree->SetRoot(selectorA));
        BOOST_CHECK_NO_THROW(asyncTree->SetRoot(selectorA));
    }

    // Return numSteps + 1(state before executing anything) states during asynchronous execution.
    std::vector<BehaviorTreeState> executeAsync(IBehaviorTreePtr tree, int numSteps) {

        BOOST_REQUIRE(tree);

        std::vector<BehaviorTreeState> treeStates(1, tree->State());

        if(numSteps < 0) {

            while(tree->State() != BehaviorTreeState::STATE_FINISHED && 
                  tree->State() != BehaviorTreeState::STATE_FAILED) {

                treeStates.push_back(tree->ExecuteAsync());
            }
        } else {

            for(int i=0; i<numSteps; ++i) {
                treeStates.push_back(tree->ExecuteAsync());
            }
        }
        
        return treeStates;
    }

    AIFactory  aiFactory;

    ISelectorPtr selectorA;
    ISelectorPtr selectorB;

    IActionPtr failingAction;
    IActionPtr succeedingAction;

    IBehaviorTreePtr syncTree;
    IBehaviorTreePtr asyncTree;
};

BOOST_FIXTURE_TEST_CASE(TestExceptionOnInvalidRoot, AIAsyncFixture) {

    // Synchronous case
    syncTree->SetRoot(nullptr);
    asyncTree->SetRoot(nullptr);

    BOOST_CHECK_THROW(syncTree->ExecuteSync(), std::invalid_argument);
    BOOST_CHECK_THROW(syncTree->ExecuteAsync(), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(TestAsyncExecutionStepCount, AIAsyncFixture) {

    // First 5 steps should flag the tree in "running" state.
    // After the "succeeding" action is evaluated the tree's state
    // must be "finished".
    const std::vector<BehaviorTreeState> expectedStates = 
        boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_FINISHED);
    
    auto treeStates = executeAsync(asyncTree, -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
                                  treeStates.begin(), treeStates.end());
}



} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw