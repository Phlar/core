#pragma once

#include "AIFwdDeclarations.hpp"
#include "IAction.hpp"

#include "Task.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Action : public base::InterfaceImpl<IAction>, public Task {

    public:

        Action(const ActionFnc& action);
        virtual ~Action();

        void SetAction(const ActionFnc& action) override;

    protected:

        //! \brief Returns result of the provided action function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! action function provided.
        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;

        ActionFnc m_action;
};

typedef boost::intrusive_ptr<Action> ActionPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw