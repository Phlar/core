#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIDecorators"

#include "Action.hpp"
#include "IAction.hpp"
#include "IBlackboard.hpp"
#include "IDecorator.hpp"
#include "IInverter.hpp"
#include "IRepeater.hpp"

#include "RepeaterUtils.hpp"

#include <boost/test/unit_test.hpp>

#include "AIServiceFixture.hpp"


namespace aw {
namespace core {
namespace ai {
namespace testing {

class TestAction : public impl::Action {

    public:

        TestAction(ITask::TaskResult& resultToReturn)
        : Action(ActionFnc())
        , m_resultToReturn(resultToReturn)
        , m_timesEvaluated(0) {
        }

        virtual ~TestAction() {
        }

        virtual ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* /*yield*/) const {

            m_timesEvaluated++;
            return m_resultToReturn;
        };

        uint16_t GetTimesEvaluated() const {

            return m_timesEvaluated;
        }

    protected:

        mutable uint16_t    m_timesEvaluated;
        ITask::TaskResult& m_resultToReturn;
};
typedef boost::intrusive_ptr<TestAction> TestActionPtr;


BOOST_FIXTURE_TEST_CASE(TestInverter, AIServiceFixture) {

    IBlackboardPtr blackboard = aiService->createBlackboard();

    ITask::TaskResult resultToReturn = ITask::TaskResult::TASK_RESULT_FAILED;
    IActionPtr testAction = boost::intrusive_ptr<IAction>(new TestAction(resultToReturn));

    IInverterPtr inverter = aiService->createInverter();
    inverter->SetDecoratedTask(testAction);
    BOOST_CHECK_EQUAL(inverter->GetDecoratedTask(), testAction);

    // Check inversion...
    resultToReturn = ITask::TaskResult::TASK_RESULT_FAILED;
    ITask::TaskResult result = inverter->Evaluate(blackboard);
    BOOST_CHECK(result == ITask::TaskResult::TASK_RESULT_PASSED);

    // ...and the other way around
    resultToReturn = ITask::TaskResult::TASK_RESULT_PASSED;
    result = inverter->Evaluate(blackboard);
    BOOST_CHECK(result == ITask::TaskResult::TASK_RESULT_FAILED);
}

BOOST_FIXTURE_TEST_CASE(TestRepeaterCounter, AIServiceFixture) {

    IBlackboardPtr blackboard = aiService->createBlackboard();

    // Leaf action
    ITask::TaskResult resultToReturn = ITask::TaskResult::TASK_RESULT_PASSED;
    TestActionPtr testAction = boost::intrusive_ptr<TestAction>(new TestAction(resultToReturn));

    // Repeater - in this case utilizing a counter condition.
    const uint16_t timesToRunAction = 42;

    RepeatConditionUPtr repeaterCondition;
    BOOST_REQUIRE_NO_THROW(repeaterCondition = std::move(
        std::unique_ptr<RepeatConditionCounter>(new RepeatConditionCounter(timesToRunAction))));

    IRepeaterPtr repeater = aiService->createRepeater();
    repeater->SetRepeatCondition(std::move(repeaterCondition));
    repeater->SetDecoratedTask(testAction);

    // Before running the action should not be evaluated so far:
    BOOST_CHECK_EQUAL(testAction->GetTimesEvaluated(), 0);

    ITask::TaskResult result = repeater->Evaluate(blackboard);

    // Repeater task should return success in all cases.
    BOOST_CHECK(result == ITask::TaskResult::TASK_RESULT_PASSED);

    // And the sub-task should be run n-times:
    BOOST_CHECK_EQUAL(testAction->GetTimesEvaluated(), timesToRunAction);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw