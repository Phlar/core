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

#pragma warning( disable: 4702 )

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
        BOOST_CHECK_NO_THROW(throwingAction = aiFactory.createAction([](IBlackboardPtr /*blakboard*/){
            throw std::exception("Error in action.");
            return TaskResult::TASK_RESULT_PASSED;
        }));
        BOOST_REQUIRE(failingAction);
        BOOST_REQUIRE(succeedingAction);
        BOOST_REQUIRE(throwingAction);

        BOOST_CHECK_NO_THROW(throwingCondition = aiFactory.createCondition([](IBlackboardPtr /*blakboard*/){
            throw std::exception("Error in condition.");
            return TaskResult::TASK_RESULT_PASSED;
        }));
        BOOST_REQUIRE(throwingCondition);

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

            do {

                BOOST_CHECK_NO_THROW(treeStates.push_back(tree->ExecuteAsync()));

            } while(tree->State() != BehaviorTreeState::STATE_FINISHED && 
                    tree->State() != BehaviorTreeState::STATE_FAILED);

        } else {

            for(int i=0; i<numSteps; ++i) {
                BOOST_CHECK_NO_THROW(treeStates.push_back(tree->ExecuteAsync()));
            }
        }
        
        return treeStates;
    }

    void replaceSelectorBSucceedingAction(ITaskPtr task) {

        BOOST_REQUIRE(task);

        BOOST_CHECK_NO_THROW(selectorB->RemoveTasks(succeedingAction));
        BOOST_CHECK_EQUAL(selectorB->GetNumTasks(), 1);
        BOOST_CHECK_NO_THROW(selectorB->AddTask(task));
        BOOST_CHECK_EQUAL(selectorB->GetNumTasks(), 2);
    }

    AIFactory  aiFactory;

    ISelectorPtr selectorA;
    ISelectorPtr selectorB;

    IActionPtr failingAction;
    IActionPtr succeedingAction;
    IActionPtr throwingAction;

    IConditionPtr throwingCondition;

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

BOOST_FIXTURE_TEST_CASE(TestSyncExecution, AIAsyncFixture) {

    // Check flags in the beginning and in the end.
    BOOST_CHECK_EQUAL(syncTree->State(), BehaviorTreeState::STATE_NOT_RUN);
    BehaviorTreeState result = BehaviorTreeState::STATE_NOT_RUN;
    BOOST_CHECK_NO_THROW(result = syncTree->ExecuteSync());
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_FINISHED);
    BOOST_CHECK_EQUAL(syncTree->State(), BehaviorTreeState::STATE_FINISHED);
}

BOOST_FIXTURE_TEST_CASE(TestAsyncAndSyncExecution, AIAsyncFixture) {

    // Execute some steps asynchronously and then continue synchronously for the rest.
    const std::vector<BehaviorTreeState> expectedStates = 
        boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_RUNNING);

    auto treeStates = executeAsync(asyncTree, 2);
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
                                  treeStates.begin(), treeStates.end());
    BOOST_CHECK_EQUAL(asyncTree->State(), BehaviorTreeState::STATE_RUNNING);

    // Now in the middle of the execution, continue synchronously.
    BehaviorTreeState syncResult = BehaviorTreeState::STATE_RUNNING;
    BOOST_CHECK_NO_THROW(syncResult = asyncTree->ExecuteSync());
    BOOST_CHECK_EQUAL(syncResult, BehaviorTreeState::STATE_FINISHED);
}

BOOST_FIXTURE_TEST_CASE(TestAsyncExecutionAndReset, AIAsyncFixture) {

    // Execute some steps asynchronously and then reset all.
    {
        const std::vector<BehaviorTreeState> expectedStates = 
            boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
            (BehaviorTreeState::STATE_RUNNING);

        auto treeStates = executeAsync(asyncTree, 2);
        BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
            treeStates.begin(), treeStates.end());
        BOOST_CHECK_EQUAL(asyncTree->State(), BehaviorTreeState::STATE_RUNNING);
    }

    // Now reset all.
    BOOST_CHECK_NO_THROW(asyncTree->ResetAsyncExecution());
    BOOST_CHECK_EQUAL(asyncTree->State(), BehaviorTreeState::STATE_NOT_RUN);

    // And run everything again.
    {
        const std::vector<BehaviorTreeState> expectedStates = 
            boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
                                  (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
                                  (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
                                  (BehaviorTreeState::STATE_FINISHED);

        auto treeStates = executeAsync(asyncTree, -1);
        BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
            treeStates.begin(), treeStates.end());
        BOOST_CHECK_EQUAL(asyncTree->State(), BehaviorTreeState::STATE_FINISHED);
    }
}

BOOST_FIXTURE_TEST_CASE(TestSyncSucceedingExecutionMultipleTimes, AIAsyncFixture) {

}

BOOST_FIXTURE_TEST_CASE(TestAsyncSucceedingExecutionMultipleTimes, AIAsyncFixture) {

    auto runAllAsync = [&](IBehaviorTreePtr tree) {

        const std::vector<BehaviorTreeState> expectedStates = 
            boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
            (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
            (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
            (BehaviorTreeState::STATE_FINISHED);

        auto treeStates = executeAsync(tree, -1);
        BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
            treeStates.begin(), treeStates.end());
    };

    runAllAsync(asyncTree);

    //! Triggering another asynchronous-execution call should do nothing - does not start again
    //! as it requires an explicit reset by the user.
    BehaviorTreeState result = asyncTree->State();
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_FINISHED);
    BOOST_CHECK_NO_THROW(result = asyncTree->ExecuteAsync());
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_FINISHED);

    BOOST_CHECK_NO_THROW(asyncTree->ResetAsyncExecution());

    runAllAsync(asyncTree);
}

BOOST_FIXTURE_TEST_CASE(TestSyncExecutionThrowInAction, AIAsyncFixture) {

    replaceSelectorBSucceedingAction(throwingAction);

    BehaviorTreeState result = syncTree->State();
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_NOT_RUN);
    BOOST_CHECK_NO_THROW(result = syncTree->ExecuteSync());
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_FAILED);
}

BOOST_FIXTURE_TEST_CASE(TestSyncExecutionThrowInCondition, AIAsyncFixture) {

    replaceSelectorBSucceedingAction(throwingCondition);

    BehaviorTreeState result = syncTree->State();
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_NOT_RUN);
    BOOST_CHECK_NO_THROW(result = syncTree->ExecuteSync());
    BOOST_CHECK_EQUAL(result, BehaviorTreeState::STATE_FAILED);
}

BOOST_FIXTURE_TEST_CASE(TestAsyncExecutionThrowInAction, AIAsyncFixture) {

    replaceSelectorBSucceedingAction(throwingAction);

    const std::vector<BehaviorTreeState> expectedStates = 
        boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
                              (BehaviorTreeState::STATE_FAILED);

    auto treeStates = executeAsync(asyncTree, -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
        treeStates.begin(), treeStates.end());

    // In case of a failure trying to run the tree again, should do nothing,
    // i.e. explicit reset required.
    BOOST_CHECK_EQUAL(asyncTree->ExecuteAsync(), BehaviorTreeState::STATE_FAILED);
    BOOST_CHECK_NO_THROW(asyncTree->ResetAsyncExecution());
    BOOST_CHECK_EQUAL(asyncTree->State(), BehaviorTreeState::STATE_NOT_RUN);
}

BOOST_FIXTURE_TEST_CASE(TestAsyncExecutionThrowInCondition, AIAsyncFixture) {

    replaceSelectorBSucceedingAction(throwingCondition);

    const std::vector<BehaviorTreeState> expectedStates = 
        boost::assign::list_of(BehaviorTreeState::STATE_NOT_RUN)(BehaviorTreeState::STATE_RUNNING)
        (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
        (BehaviorTreeState::STATE_RUNNING)(BehaviorTreeState::STATE_RUNNING)
        (BehaviorTreeState::STATE_FAILED);

    auto treeStates = executeAsync(asyncTree, -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedStates.begin(), expectedStates.end(),
        treeStates.begin(), treeStates.end());

    // In case of a failure trying to run the tree again, should do nothing,
    // i.e. explicit reset required.
    BOOST_CHECK_EQUAL(asyncTree->ExecuteAsync(), BehaviorTreeState::STATE_FAILED);
    BOOST_CHECK_NO_THROW(asyncTree->ResetAsyncExecution());
    BOOST_CHECK_EQUAL(asyncTree->State(), BehaviorTreeState::STATE_NOT_RUN);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw

#pragma warning( default: 4702 )
