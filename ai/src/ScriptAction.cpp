#include "ScriptAction.hpp"

#include "IBlackboard.hpp"

#include "IService.hpp"
#include "ServiceLocator.hpp"
#include "IScriptingService.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


ScriptAction::ScriptProperties::ScriptProperties(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad)
: m_filePath(filePath)
, m_functionName(functionName)
, m_scriptContext(scripting::IScriptContextPtr()) {

    if(!delayLoad) {
        resolveScript();
    }
}

void ScriptAction::ScriptProperties::resolveScript() {

    try {

        // Check file's presence.
        if(boost::filesystem::exists(m_filePath)) {

            std::stringstream errorMessage;
            errorMessage << "File does not exists '" << m_filePath << "'.";
            throw std::invalid_argument(errorMessage.str());
        }

        // Retrieve the script resolver and the appropriate context.
        base::IServicePtr service = base::ServiceLocator::Instance()->GetService(scripting::ID_SCRIPTING_SERVICE);
        scripting::IScriptingServicePtr scriptingService = boost::dynamic_pointer_cast<scripting::IScriptingService>(service);
        if(!scriptingService) {

            throw std::runtime_error("Error casting abstract service to scripting-service.");
        }

        m_scriptContext = scriptingService->GetContext(m_filePath);
        if(!m_scriptContext) {
            throw std::runtime_error("Invalid script context returned.");
        }

    } catch(const std::exception& e) {

        std::stringstream errorMessage = buildErrorMessage(e.what());
        throw std::runtime_error(errorMessage.str());

    } catch( ... ) {

        std::stringstream errorMessage = buildErrorMessage(std::string("Unknown exception caught."));
        throw std::runtime_error(errorMessage.str());
    }
}

std::stringstream ScriptAction::ScriptProperties::buildErrorMessage(const std::string& reason) const {

    std::stringstream errorMessage;
    errorMessage << "Error resolving script file:" << std::endl
                 << "\t\tfile: " << m_filePath << std::endl
                 << "\t\tfunction: " << m_functionName;
    
    if(!reason.empty()) {
       errorMessage << std::endl << "\t\tReason: " << reason;
    }
    return errorMessage;
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

TaskResult ScriptAction::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* /*yield*/) const {

    if(!m_scriptProperties) {
        throw std::runtime_error("Error executing script-action due to invalid script-properties.");
    }

    if(m_scriptProperties->m_scriptContext) {

        // No valid context - we assume it's a delayed load.
        //! \todo: Log this!
        m_scriptProperties->resolveScript();
    }

    try {

        scripting::ArgumentVector args;
        args.push_back(scripting::Argument(blackboard));

        // \todo: Check about exposing TaskResult to the scripting rather than using plain ints here.
        scripting::ReturnValuesHolder returnValues = scripting::ReturnValuesHolder::Create<TaskResult>();

        m_scriptProperties->m_scriptContext->ExecuteScript(m_scriptProperties->m_functionName, args, returnValues);
        return returnValues.GetTypedValue<TaskResult>(0);

    } catch(const std::exception& e) {

        throw std::runtime_error(m_scriptProperties->buildErrorMessage(e.what()).str());

    } catch( ... ) {
        
        throw std::runtime_error(m_scriptProperties->buildErrorMessage("Unknown exception caught.").str());
    }
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw