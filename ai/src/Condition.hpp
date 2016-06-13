#pragma once

#include "AIFwdDeclarations.hpp"
#include "ICondition.hpp"

#include "RunnableTask.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Condition : public base::InterfaceImpl<ICondition>, public RunnableTask {

    public:

        Condition(const ConditionFnc& condition);
        virtual ~Condition();

        //@{
        //! \brief Implementation of IAction.
        void SetCondition(const ConditionFnc& condition) override;
        //@}

    protected:

        //! \brief Returns result of the provided evaluation function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! evaluation function provided.
        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw