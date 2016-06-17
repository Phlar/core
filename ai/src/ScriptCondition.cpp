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

ScriptCondition::~ScriptCondition() {
}

void ScriptCondition::SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) {

    setScriptFileSource(filePath, functionName, delayLoad);
}

void ScriptCondition::SetScriptString(const std::string scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) {

    setScriptStringSource(scriptSource, resolverID, functionName, delayLoad);
}


ITask::TaskResult ScriptCondition::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    return RunnableScriptTask::evaluate(blackboard, yield);
}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw