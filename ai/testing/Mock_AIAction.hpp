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

        TestAction(ActionFnc actionFnc)
        : Action(actionFnc) {
        }

        virtual ~TestAction() {
        }

        virtual TaskResult testEvaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* /*yield*/) const {

            if(m_action) {
                return m_action(blackboard);
            } else {
                return TaskResult::TASK_RESULT_FAILED;
            }
        };

    protected:
        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {
            return testEvaluate(blackboard, yield);
        }
};


MOCK_BASE_CLASS(MockAIAction, TestAction) {

    public:

        MockAIAction()
        : TestAction() {
        }

        MockAIAction(ActionFnc actionFnc) 
        : TestAction(actionFnc) {
        }

        MOCK_METHOD(testEvaluate, 2);
};

} // namespace testing	
} // namespace ai	
} // namespace core	
} // namespace aw