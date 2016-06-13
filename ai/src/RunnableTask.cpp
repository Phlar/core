#include "RunnableTask.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

RunnableTask::RunnableTask(const RunnableTaskFnc& runnableFunction, RunnableTaskType runnableTaskType)
: m_runnableFunction(runnableFunction)
, m_runnableTaskType(runnableTaskType) {
}

RunnableTask::RunnableTask()
: m_runnableTaskType(RunnableTaskType::RUNNABLE_TASK_ACTION) {   // Just to make the compiler happy...
}

RunnableTask::~RunnableTask() {
}

void RunnableTask::setRunnableTaskFunction(const RunnableTaskFnc& runnableFunction) {

    //! Todo: Log in case of an empty functor passed.
    //        "TASK_RESULT_PASSED" will be always returned in that case.
    m_runnableFunction = runnableFunction;
}

ITask::TaskResult RunnableTask::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* /*yield*/) const {

    if(m_runnableFunction) {
        return m_runnableFunction(blackboard);
    }

    return ITask::TaskResult::TASK_RESULT_PASSED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw