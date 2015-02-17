#pragma once

#include <ICondition.hpp>

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Condition : public ICondition {

    public:

        Condition(const ConditionFnc& condition);
        virtual ~Condition();

        //! \brief Returns result of the provided evaluation function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! evaluation function provided.
        TaskResult evaluate();

        void SetCondition(const ConditionFnc& condition);

    protected:

        ConditionFnc m_condition;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw