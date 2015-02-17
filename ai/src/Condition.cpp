#include "Condition.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

Condition::Condition(const ConditionFnc& condition)
: m_condition(condition) {
}

Condition::~Condition() {
}

TaskResult Condition::evaluate()
{
    if(m_condition) {
        return m_condition();
    }

    return TaskResult::TASK_RESULT_PASSED;
}

void Condition::SetCondition(const ConditionFnc& condition) {

    //! Todo: Log in case of an empty functor passed.
    //        "TASK_RESULT_PASSED" will be always returned in that case.
    m_condition = condition;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw