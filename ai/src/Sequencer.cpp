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

ITask::TaskResult Sequencer::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    for(ITaskPtr task : m_children) {
        const ITask::TaskResult result = task->Evaluate(blackboard, yield);
        if(result != ITask::TaskResult::TASK_RESULT_PASSED) {
            return result;
        }
    }
    return ITask::TaskResult::TASK_RESULT_PASSED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw