#pragma once

#include "ICondition.hpp"

#include "Task.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Condition : public base::InterfaceImpl<ICondition>, public Task {

    public:

        Condition(const ConditionFnc& condition);
        virtual ~Condition();

        //! \brief Returns result of the provided evaluation function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! evaluation function provided.
        TaskResult Evaluate() override;

        void SetCondition(const ConditionFnc& condition) override;

    protected:

        ConditionFnc m_condition;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw