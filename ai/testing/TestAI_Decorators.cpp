#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIDecorators"

#include "AIFactory.hpp"
#include "Action.hpp"
#include "IAction.hpp"
#include "IBlackboard.hpp"
#include "IDecorator.hpp"
#include "IInverter.hpp"
#include "IRepeater.hpp"

#include "RepeaterUtils.hpp"

#include <boost/test/unit_test.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

class TestAction : public impl::Action, public boost::noncopyable {

    public:

        TestAction(TaskResult& resultToReturn)
        : Action(ActionFnc())
        , m_resultToReturn(resultToReturn)
        , m_timesEvaluated(0) {
        }

        virtual ~TestAction() {
        }

        virtual TaskResult evaluate(IBlackboardPtr blackboard) const {

            m_timesEvaluated++;
            return m_resultToReturn;
        };

        uint16_t GetTimesEvaluated() const {

            return m_timesEvaluated;
        }

    protected:

        mutable uint16_t    m_timesEvaluated;
        TaskResult& m_resultToReturn;
};
typedef boost::intrusive_ptr<TestAction> TestActionPtr;


BOOST_AUTO_TEST_CASE(TestInverter) {

    AIFactory aiFactory;

    IBlackboardPtr blackboard = aiFactory.createBlackboard();

    TaskResult resultToReturn = TaskResult::TASK_RESULT_FAILED;
    IActionPtr testAction = boost::intrusive_ptr<IAction>(new TestAction(resultToReturn));

    IInverterPtr inverter = aiFactory.createInverter();
    inverter->SetDecoratedTask(testAction);
    BOOST_CHECK_EQUAL(inverter->GetDecoratedTask(), testAction);

    // Check inversion...
    resultToReturn = TaskResult::TASK_RESULT_FAILED;
    TaskResult result = inverter->Evaluate(blackboard);
    BOOST_CHECK(result == TaskResult::TASK_RESULT_PASSED);

    // ...and the other way around
    resultToReturn = TaskResult::TASK_RESULT_PASSED;
    result = inverter->Evaluate(blackboard);
    BOOST_CHECK(result == TaskResult::TASK_RESULT_FAILED);
}

BOOST_AUTO_TEST_CASE(TestRepeaterCounter) {

    AIFactory aiFactory;

    IBlackboardPtr blackboard = aiFactory.createBlackboard();

    // Leaf action
    TaskResult resultToReturn = TaskResult::TASK_RESULT_PASSED;
    TestActionPtr testAction = boost::intrusive_ptr<TestAction>(new TestAction(resultToReturn));

    // Repeater - in this case utilizing a counter condition.
    const uint16_t timesToRunAction = 42;

    RepeatConditionUPtr repeaterCondition;
    BOOST_REQUIRE_NO_THROW(repeaterCondition = std::move(
        std::unique_ptr<RepeatConditionCounter>(new RepeatConditionCounter(timesToRunAction))));

    IRepeaterPtr repeater = aiFactory.createRepeater();
    repeater->SetRepeatCondition(std::move(repeaterCondition));
    repeater->SetDecoratedTask(testAction);

    // Before running the action should not be evaluated so far:
    BOOST_CHECK_EQUAL(testAction->GetTimesEvaluated(), 0);

    TaskResult result = repeater->Evaluate(blackboard);

    // Repeater task should return success in all cases.
    BOOST_CHECK(result == TaskResult::TASK_RESULT_PASSED);

    // And the sub-task should be run n-times:
    BOOST_CHECK_EQUAL(testAction->GetTimesEvaluated(), timesToRunAction);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw