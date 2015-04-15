#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIDecorators"

#include "AIFactory.hpp"
#include "Action.hpp"
#include "IAction.hpp"
#include "IBlackboard.hpp"
#include "IDecorator.hpp"
#include "IInverter.hpp"
#include "IRepeater.hpp"

#include <boost/test/unit_test.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

class TestAction : public impl::Action, public boost::noncopyable {

    public:

        TestAction(TaskResult& resultToReturn)
        : Action(ActionFnc())
        , m_resultToReturn(resultToReturn) {
        }

        virtual ~TestAction() {
        }

        virtual TaskResult evaluate(IBlackboardPtr blackboard) const {
            return m_resultToReturn;
        };

    protected:

        TaskResult& m_resultToReturn;
};


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

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw