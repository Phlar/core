#include "InitScriptingService.hpp"

#include "ScriptingService.hpp"

#include "ServiceLocator.hpp"

namespace aw {
namespace core {
namespace scripting {

IScriptingServicePtr RegisterService(base::ServiceLocatorPtr serviceLocator) {

    if(!serviceLocator) {
        throw std::invalid_argument("Invalid service-locator to register scripting service to.");
    }

    IScriptingServicePtr scriptingService = IScriptingServicePtr(new ScriptingService());
    serviceLocator->RegisterService(scriptingService);
    return scriptingService;
}

} // namespace scripting
} // namespace core
} // namespace aw