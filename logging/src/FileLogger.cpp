#include "LoggingService.hpp"

/*
#include <boost/format.hpp>
#include <boost/uuid/string_generator.hpp>

#include <sstream>


namespace aw {
namespace core {
namespace scripting {

const base::UUID ScriptingService::serviceID = boost::uuids::string_generator()("{237C7009-A671-4509-99E9-C9AC6C0BB78F}");

ScriptingService::ScriptingService()
: m_resolvers() {
}

ScriptingService::~ScriptingService() {
}

base::UUID ScriptingService::GetServiceID() const {

    return serviceID;
}

void ScriptingService::AddResolver(IScriptResolverPtr resolver) {

    if(!resolver) {
        throw std::invalid_argument("Invalid script resolver provided.");
    }

    if(!m_resolvers.insert(resolver).second) {
        throw std::logic_error("Resolver already registered.");
    }
}

IScriptContextPtr ScriptingService::GetContext(const boost::filesystem::path& scriptPath) const {

    // Before asking resolvers first check the validity of the file.
    boost::system::error_code errorCode;
    if(!boost::filesystem::exists(scriptPath, errorCode) || errorCode) {

        std::stringstream errorMessage;
        errorMessage << "Error while checking existing file '"
                     << scriptPath.string() << "'";

        if(errorCode) {
            errorMessage << " (error code: " << errorCode << ")";
        }

        throw std::invalid_argument(errorMessage.str());
    }

    // Walk all resolvers and take the first matching one.
    for(const IScriptResolverPtr& resolver : m_resolvers) {

        if(resolver->IsFileSupported(scriptPath)) {
            return resolver->GetContext(scriptPath);
        }
    }
    return IScriptContextPtr();
}

} // namespace scripting
} // namespace core
} // namespace aw
*/