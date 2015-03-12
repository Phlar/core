#include "Sequencer.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

Sequencer::Sequencer() {
}

Sequencer::~Sequencer() {
}

TaskResult Sequencer::evaluate() {
    
    for(ITaskPtr task : m_children) {

        const TaskResult result = task->Evaluate();
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