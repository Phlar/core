#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"
#include "IBlackboard.hpp"
#include "Action.hpp"

#include <turtle/mock.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

//! Todo: Check means how to properly mock 'non-public' methods without
//! the introduction of the class below.
class TestAction : public impl::Action {

    public:
        TestAction()
        : Action(ActionFnc()) {
        }

        virtual ~TestAction() {
        }

        virtual TaskResult testEvaluate(IBlackboardPtr blackboard) const {
            return TaskResult::TASK_RESULT_FAILED;
        };

    protected:
        TaskResult evaluate(IBlackboardPtr blackboard) const {
            return testEvaluate(blackboard);
        }
};



MOCK_BASE_CLASS(MockAIAction, TestAction) {

    MOCK_METHOD(testEvaluate, 1);
};

} // namespace testing	
} // namespace ai	
} // namespace core	
} // namespace aw