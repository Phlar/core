#include "InitScriptingService.hpp"

#include "ScriptingService.hpp"

#include "ServiceLocator.hpp"

namespace aw {
namespace core {
namespace scripting {

void RegisterService(base::ServiceLocatorPtr serviceLocator) {

	if(!serviceLocator) {
		throw std::invalid_argument("Invalid service-locator to register scripting service to.");
	}
	
	serviceLocator->RegisterService(IScriptingServicePtr(new ScriptingService()));
}

} // namespace scripting
} // namespace core
} // namespace aw