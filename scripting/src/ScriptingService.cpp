#include "ScriptingService.hpp"

#include <boost/format.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <sstream>


namespace aw {
namespace core {
namespace scripting {


ScriptingService::ScriptingService()
: m_resolvers() {
}

ScriptingService::~ScriptingService() {
}

base::UUID ScriptingService::GetServiceID() const {

    return ID_SCRIPTING_SERVICE;
}

void ScriptingService::AddResolver(IScriptResolverPtr resolver) {

    if(!resolver) {
        throw std::invalid_argument("Invalid script resolver provided.");
    }

    if(!m_resolvers.insert(std::make_pair(resolver->GetResolverID(), resolver)).second) {

        std::stringstream errorMessage;
        errorMessage << "Resolver '" << resolver->GetResolverID() << "already registered.";
        throw std::logic_error(errorMessage.str());
    }
}

IScriptResolverPtr ScriptingService::GetResolver(const base::UUID& resolverID) const {

    auto findIter = m_resolvers.find(resolverID);
    if(findIter == m_resolvers.end()) {
        // todo: Log this in debug!
        return IScriptResolverPtr();
    }
    return findIter->second;
}

IScriptContextPtr ScriptingService::GetContextFromFile(const boost::filesystem::path& scriptPath) const {

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
    for(auto resolverEntry : m_resolvers) {

        if(resolverEntry.second->IsFileSupported(scriptPath)) {
            return resolverEntry.second->GetContextFromFile(scriptPath);
        }
    }
    return IScriptContextPtr();
}

} // namespace scripting
} // namespace core
} // namespace aw