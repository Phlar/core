#include "Sequencer.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

Sequencer::Sequencer() {
}

Sequencer::~Sequencer() {
}

TaskResult Sequencer::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    for(ITaskPtr task : m_children) {
        const TaskResult result = task->Evaluate(blackboard, yield);
        if(result != TaskResult::TASK_RESULT_PASSED) {
            return result;
        }
    }
    return TaskResult::TASK_RESULT_PASSED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw