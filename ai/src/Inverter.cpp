#include "Inverter.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

Inverter::Inverter() {
}

Inverter::~Inverter() {
}

TaskResult Inverter::evaluate(IBlackboardPtr blackboard) const {

    TaskResult result = evaluateDecoratedTask(blackboard);
    if(result == TaskResult::TASK_RESULT_PASSED) {
        return TaskResult::TASK_RESULT_FAILED;
    } else if(result == TaskResult::TASK_RESULT_FAILED) {
        return TaskResult::TASK_RESULT_PASSED;
    }

    return result;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw