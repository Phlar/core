#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIBasics"

#include <iostream>
#include <boost/test/unit_test.hpp>

#include "AIFactory.hpp"
#include "IAction.hpp"
#include "IBlackboard.hpp"
#include "ICondition.hpp"
#include "ITaskContainer.hpp"
#include "TaskContainer.hpp"

#include "InterfaceImpl.hpp"

namespace aw {
namespace core {
namespace ai {
namespace testing {

class IInstanceableTaskContainer : public virtual ITaskContainer {
};
typedef boost::intrusive_ptr<IInstanceableTaskContainer> IInstanceableTaskContainerPtr;

class InstanceableTaskContainer : public base::InterfaceImpl<IInstanceableTaskContainer>, public impl::TaskContainer {

    public: 
        InstanceableTaskContainer() {
        }

        virtual ~InstanceableTaskContainer() {
        }

    protected:

        TaskResult evaluate(IBlackboardPtr blackboard) const override {

            return TaskResult::TASK_RESULT_PASSED;
        }
};

struct AIBasicsFixture {

    AIBasicsFixture() {

        BOOST_CHECK_NO_THROW(validAction = aiFactory.createAction(ActionFnc()));
        BOOST_CHECK_NO_THROW(validCondition = aiFactory.createCondition(ConditionFnc()));

        BOOST_REQUIRE(validAction);
        BOOST_REQUIRE(validCondition);

        BOOST_REQUIRE(!invalidAction);
        BOOST_REQUIRE(!invalidCondition);

        taskContainer = IInstanceableTaskContainerPtr(new InstanceableTaskContainer());
        BOOST_REQUIRE(taskContainer);
    }

    AIFactory aiFactory;
    IActionPtr validAction;
    IConditionPtr validCondition;

    IActionPtr invalidAction;
    IConditionPtr invalidCondition;

    IInstanceableTaskContainerPtr taskContainer;
};


BOOST_FIXTURE_TEST_CASE(TestEmptyContainer, AIBasicsFixture) {

    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 0);
}

BOOST_FIXTURE_TEST_CASE(TestAddingTasks, AIBasicsFixture) {

    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 0);

    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validAction));
    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validCondition));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 2);

    BOOST_CHECK_THROW(taskContainer->AddTask(invalidAction), std::invalid_argument);
    BOOST_CHECK_THROW(taskContainer->AddTask(invalidCondition), std::invalid_argument);
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 2);
}

BOOST_FIXTURE_TEST_CASE(TestRemoveListedTasks, AIBasicsFixture) {

    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validAction));
    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validCondition));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 2);

    BOOST_CHECK_NO_THROW(taskContainer->RemoveTasks(validAction));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 1);

    BOOST_CHECK_NO_THROW(taskContainer->RemoveTasks(validCondition));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 0);
}

BOOST_FIXTURE_TEST_CASE(TestRemoveDuplicatedTasks, AIBasicsFixture) {

    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validCondition));
    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validAction));
    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validCondition));
    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validCondition));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 4);

    BOOST_CHECK_NO_THROW(taskContainer->RemoveTasks(validCondition));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 1);

}

BOOST_FIXTURE_TEST_CASE(TestRemoveUnknownTasks, AIBasicsFixture) {

    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validAction));
    BOOST_CHECK_NO_THROW(taskContainer->AddTask(validCondition));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 2);

    // Remove a valid task which is not listed.
    IActionPtr notListedAction = aiFactory.createAction(ActionFnc());
    BOOST_REQUIRE(notListedAction);
    BOOST_CHECK_NO_THROW(taskContainer->RemoveTasks(notListedAction));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 2);

    // Remove an invalid task.
    BOOST_CHECK_NO_THROW(taskContainer->RemoveTasks(invalidAction));
    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 2);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw