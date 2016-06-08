#include "Action.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

Action::Action(const ActionFnc& action)
: RunnableTask(action, RunnableTask::RunnableTaskType::RUNNABLE_TASK_ACTION) {
}

Action::~Action() {
}

void Action::SetAction(const ActionFnc& action) {

    setRunnableTaskFunction(action);
}

TaskResult Action::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    return RunnableTask::evaluate(blackboard, yield);
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw