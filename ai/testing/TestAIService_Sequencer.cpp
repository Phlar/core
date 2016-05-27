#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AISequencer"

#include "ISequencer.hpp"

#include "Mock_AIAction.hpp"

#include <boost/test/unit_test.hpp>

#include "AIServiceFixture.hpp"

namespace aw {
namespace core {
namespace ai {
namespace testing {


BOOST_FIXTURE_TEST_CASE(TestSequencerExecution, AIServiceFixture) {

    ISequencerPtr sequencer = aiService->createSequencer();
    BOOST_REQUIRE(sequencer);

    boost::intrusive_ptr<MockAIAction> mockAction(new MockAIAction());
    MOCK_EXPECT(mockAction->testEvaluate).exactly(3).returns(TaskResult::TASK_RESULT_PASSED);
   
    sequencer->AddTask(mockAction);
    sequencer->AddTask(mockAction);
    sequencer->AddTask(mockAction);

    sequencer->Evaluate(IBlackboardPtr());
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw