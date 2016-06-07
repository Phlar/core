#include "Condition.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

Condition::Condition(const ConditionFnc& condition)
: RunnableTask(condition, RunnableTask::RunnableTaskType::RUNNABLE_TASK_CONDITION) {
}

Condition::~Condition() {
}

void Condition::SetCondition(const ConditionFnc& condition) {

    SetRunnableTaskFunction(condition);
}

TaskResult Condition::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    return RunnableTask::evaluate(blackboard, yield);
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw