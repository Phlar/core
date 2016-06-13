#include "ScriptCondition.hpp"

#include "IBlackboard.hpp"

#include "IService.hpp"
#include "ServiceLocator.hpp"
#include "IScriptingService.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


ScriptCondition::ScriptCondition()
: RunnableScriptTask(RunnableScriptTask::RunnableScriptTaskType::RUNNABLE_SCRIPT_TASK_CONDITION) {
}

ScriptCondition::ScriptCondition(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad)
: RunnableScriptTask(RunnableScriptTask::RunnableScriptTaskType::RUNNABLE_SCRIPT_TASK_CONDITION, filePath, functionName, delayLoad) {
}

ScriptCondition::~ScriptCondition() {
}

void ScriptCondition::SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) {

    setScriptFile(filePath, functionName, delayLoad);
}

ITask::TaskResult ScriptCondition::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    return RunnableScriptTask::evaluate(blackboard, yield);
}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw