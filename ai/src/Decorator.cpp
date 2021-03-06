#include "Decorator.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {


Decorator::Decorator(ITaskPtr decoratedTask)
: m_decoratedTask(decoratedTask) {
}

Decorator::~Decorator() {
}

void Decorator::SetDecoratedTask(ITaskPtr task) {

    // Todo: Add logging in case of switching to an "empty" task.
    m_decoratedTask = task;
}

ITaskPtr Decorator::GetDecoratedTask() const {

    return m_decoratedTask;
}

ITask::TaskResult Decorator::evaluateDecoratedTask(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {
    
    if(!m_decoratedTask) {

        // Log this.
        return ITask::TaskResult::TASK_RESULT_PASSED;
    }

    return m_decoratedTask->Evaluate(blackboard, yield);
}


} // namespace impl
} // namespace core
} // namespace ai
} // namespace aw