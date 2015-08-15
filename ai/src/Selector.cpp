#include "Selector.hpp"

#include "IBlackboard.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

Selector::Selector() {
}

Selector::~Selector() {
}

TaskResult Selector::evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType& yield) const {

    if(m_children.empty()) {

        return TaskResult::TASK_RESULT_PASSED;
    }

    for(ITaskPtr task : m_children) {

        const TaskResult result = task->Evaluate(blackboard, yield);
        if(result == TaskResult::TASK_RESULT_PASSED) {
            return result;
        }
    }

    return TaskResult::TASK_RESULT_FAILED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw