#pragma once

#include "AIFwdDeclarations.hpp"
#include "IAction.hpp"

#include "RunnableTask.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Action : public base::InterfaceImpl<IAction>, public RunnableTask {

    public:

        Action(const ActionFnc& action);
        virtual ~Action();

        //@{
        //! \brief Implementation of IAction.
        void SetAction(const ActionFnc& action) override;
        //@}

    protected:

        //! \brief Returns result of the provided evaluation function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! evaluation function provided.
        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw