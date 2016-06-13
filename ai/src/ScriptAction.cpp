#include "ScriptAction.hpp"

#include "IBlackboard.hpp"

#include "IService.hpp"
#include "ServiceLocator.hpp"
#include "IScriptingService.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


ScriptAction::ScriptAction()
: RunnableScriptTask(RunnableScriptTask::RunnableScriptTaskType::RUNNABLE_SCRIPT_TASK_ACTION) {
}

ScriptAction::ScriptAction(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad)
: RunnableScriptTask(RunnableScriptTask::RunnableScriptTaskType::RUNNABLE_SCRIPT_TASK_ACTION, filePath, functionName, delayLoad) {
}
 
ScriptAction::~ScriptAction() {
}

void ScriptAction::SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) {

    setScriptFile(filePath, functionName, delayLoad);
}

ITask::TaskResult ScriptAction::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    return RunnableScriptTask::evaluate(blackboard, yield);
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw