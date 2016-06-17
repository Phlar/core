#include "RunnableScriptTask.hpp"

#include "IBlackboard.hpp"

#include "IService.hpp"
#include "ServiceLocator.hpp"
#include "IScriptingService.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


RunnableScriptTask::ScriptProperties::ScriptProperties(const std::string& functionName, const std::string& scriptSource)
: m_functionName(functionName)
, m_scriptSource(scriptSource)
, m_scriptContext(scripting::IScriptContextPtr()) {
}

RunnableScriptTask::ScriptProperties::~ScriptProperties() {
}


RunnableScriptTask::ScriptFileProperties::ScriptFileProperties(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad)
: ScriptProperties(functionName)
, m_filePath(filePath) {

    if(!delayLoad) {
        resolveScript();
    }
}

RunnableScriptTask::ScriptFileProperties::~ScriptFileProperties() {
}

void RunnableScriptTask::ScriptFileProperties::resolveScript() {

    auto buildErrorMessage = [this](const std::string& reason) -> std::string {

        std::stringstream errorMessage;

        errorMessage << "Error resolving script from file:" << std::endl
                     << buildPropertiesMessage();
        if(!reason.empty()) {
            errorMessage << std::endl << "Reason: " << reason;
        }
        return errorMessage.str();
    };

    try {

        // Check file's presence.
        if(!boost::filesystem::exists(m_filePath)) {

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

        m_scriptContext = scriptingService->GetContextFromFile(m_filePath);
        if(!m_scriptContext) {
            throw std::runtime_error("Invalid script context returned.");
        }

    } catch(const std::exception& e) {

        throw std::runtime_error(buildErrorMessage(e.what()));

    } catch( ... ) {

        throw std::runtime_error(buildErrorMessage("Unknown exception caught."));
    }
}

std::string RunnableScriptTask::ScriptFileProperties::buildPropertiesMessage() {

    std::stringstream propertiesMesssage;
    propertiesMesssage << "file: " << m_filePath << std::endl
                       << "function: " << m_functionName << std::endl
                       << "source: " << m_scriptSource;
    return propertiesMesssage.str();
}


RunnableScriptTask::ScriptStringProperties::ScriptStringProperties(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad)
: ScriptProperties(functionName, scriptSource)
, m_resolverID(resolverID) {

    if(!delayLoad) {
        resolveScript();
    }
}

RunnableScriptTask::ScriptStringProperties::~ScriptStringProperties() {
}

void RunnableScriptTask::ScriptStringProperties::resolveScript() {

    auto buildErrorMessage = [this](const std::string& reason) -> std::string {

        std::stringstream errorMessage;
        errorMessage << "Error resolving script from string:" << std::endl
                     << buildPropertiesMessage();
        if(!reason.empty()) {
            errorMessage << std::endl << "Reason: " << reason;
        }
        return errorMessage.str();
    };

    try {

        if(m_scriptSource.empty()) {

            throw std::invalid_argument("Script must not be empty.");
        }

        // Retrieve the script resolver and the appropriate context.
        base::IServicePtr service = base::ServiceLocator::Instance()->GetService(scripting::ID_SCRIPTING_SERVICE);
        scripting::IScriptingServicePtr scriptingService = boost::dynamic_pointer_cast<scripting::IScriptingService>(service);
        if(!scriptingService) {

            throw std::runtime_error("Error casting abstract service to scripting-service.");
        }

        scripting::IScriptResolverPtr scriptResolver = scriptingService->GetResolver(m_resolverID);
        if(!scriptResolver) {

            throw std::runtime_error("Error retrieving matching script-resolver.");
        }

        m_scriptContext = scriptResolver->GetContextFromString(m_scriptSource);
        if(!m_scriptContext) {
            throw std::runtime_error("Invalid script context returned.");
        }

    } catch(const std::exception& e) {

        throw std::runtime_error(buildErrorMessage(e.what()));

    } catch( ... ) {

        throw std::runtime_error(buildErrorMessage("Unknown exception caught."));
    }
}

std::string RunnableScriptTask::ScriptStringProperties::buildPropertiesMessage() {

    std::stringstream propertiesMesssage;
    propertiesMesssage << "source: " << m_scriptSource << std::endl
                       << "function: " << m_functionName << std::endl
                       << "resolver id: " << m_resolverID;
    return propertiesMesssage.str();
};



RunnableScriptTask::RunnableScriptTask(RunnableScriptTaskType runnableScriptTaskType)
: m_runnableScriptTaskType(runnableScriptTaskType)
, m_scriptProperties(nullptr) {
}

RunnableScriptTask::RunnableScriptTask()
: m_runnableScriptTaskType(RunnableScriptTaskType::RUNNABLE_SCRIPT_TASK_ACTION) {   // Just to make the compiler happy...
}

RunnableScriptTask::~RunnableScriptTask() {
}

ITask::TaskResult RunnableScriptTask::evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* /*yield*/) const {

    auto buildErrorMessage = [this](const std::string& resaon) -> std::string {

        std::stringstream errorMessage;
        errorMessage << "Error executing script:" << std::endl 
                     << m_scriptProperties->buildPropertiesMessage();
        if(!resaon.empty()) {
            errorMessage << std::endl << "Reason: " << resaon;
        }
        return errorMessage.str();
    };

    if(!m_scriptProperties) {
        throw std::runtime_error("Error executing script-task due to invalid script-properties.");
    }

    if(!m_scriptProperties->m_scriptContext) {

        // No valid context - we assume it's a delayed load.
        //! \todo: Log this!
        m_scriptProperties->resolveScript();
    }

    try {

        scripting::ArgumentVector args;
        args.push_back(scripting::Argument(blackboard));

        // \todo: Check about exposing ITask::TaskResult to the scripting rather than using plain ints here.
        scripting::ReturnValuesHolder returnValues = scripting::ReturnValuesHolder::Create<ITask::TaskResult>();

        m_scriptProperties->m_scriptContext->ExecuteScript(m_scriptProperties->m_functionName, args, returnValues);
        return returnValues.GetTypedValue<ITask::TaskResult>(0);

    } catch(const std::exception& e) {

        throw std::runtime_error(buildErrorMessage(e.what()));

    } catch( ... ) {

        throw std::runtime_error(buildErrorMessage("Unknown exception caught."));
    }
}

void RunnableScriptTask::setScriptFileSource(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) {

    //! \todo Clarify whether to gracefully shut down the previous script first...?
    if(m_scriptProperties) {
        //! \todo Log this!
    }

    m_scriptProperties = ScriptPropertiesPtr(new ScriptFileProperties(filePath, functionName, delayLoad));
}

void RunnableScriptTask::setScriptStringSource(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) {

    //! \todo Clarify whether to gracefully shut down the previous script first...?
    if(m_scriptProperties) {
        //! \todo Log this!
    }

    m_scriptProperties = ScriptPropertiesPtr(new ScriptStringProperties(scriptSource, resolverID, functionName, delayLoad));
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
