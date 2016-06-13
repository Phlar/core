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

ITask::TaskResult Inverter::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    ITask::TaskResult result = evaluateDecoratedTask(blackboard, yield);
    if(result == ITask::TaskResult::TASK_RESULT_PASSED) {
        return ITask::TaskResult::TASK_RESULT_FAILED;
    } else if(result == ITask::TaskResult::TASK_RESULT_FAILED) {
        return ITask::TaskResult::TASK_RESULT_PASSED;
    }

    return result;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw