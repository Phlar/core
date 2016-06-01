#include "ScriptAction.hpp"

#include "IBlackboard.hpp"

#include "ServiceLocator.hpp"
#include "IScriptingService.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

ScriptAction::ScriptProperties::ScriptProperties(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad)
: m_delayLoad(delayLoad)
, m_scriptFilePath(filePath)
, m_scriptFunction(functionName)
, m_scriptContext(scripting::IScriptContextPtr()) {
}

void ScriptAction::ScriptProperties::resolveScript() {
}


ScriptAction::ScriptAction()
: Action(ActionFnc())
, m_scriptProperties(nullptr) {
}

ScriptAction::~ScriptAction() {
}

void ScriptAction::SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) {

    //! \todo Clarify whether to gracefully shut down the previous script first...?
    if(m_scriptProperties) {
        //! \todo Log this!
    }

    m_scriptProperties = std::unique_ptr<ScriptProperties>(new ScriptProperties(filePath, functionName, delayLoad));
}


TaskResult ScriptAction::evaluate(IBlackboardPtr /*blackboard*/, TaskCoroutinePullType* /*yield*/) const {

    return TaskResult::TASK_RESULT_FAILED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw