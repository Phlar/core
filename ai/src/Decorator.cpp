#include "Decorator.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {


Decorator::Decorator()
: m_task() {
}

Decorator::~Decorator() {
}

void Decorator::SetTask(ITaskPtr task) {

    // Todo: Add logging in case of switching to an "empty" task.
    m_task = task;
}

TaskResult Decorator::evaluate() {

    // Todo: In case of an empty task - should the pre -/ post-events be
    // generated at all?
    if(!m_task) {
        return TaskResult::TASK_RESULT_PASSED;
    }

    preEvaluate();

    TaskResult result = m_task->evaluate();

    postEvaluate(result);

    return result;
}

} // namespace impl
} // namespace core
} // namespace ai
} // namespace aw