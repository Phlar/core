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

ITask::TaskResult Selector::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    if(m_children.empty()) {

        return ITask::TaskResult::TASK_RESULT_PASSED;
    }

    for(ITaskPtr task : m_children) {

        const ITask::TaskResult result = task->Evaluate(blackboard, yield);
        if(result == ITask::TaskResult::TASK_RESULT_PASSED) {
            return result;
        }
    }

    return ITask::TaskResult::TASK_RESULT_FAILED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw