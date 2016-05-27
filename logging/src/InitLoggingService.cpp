#include "InitLoggingService.hpp"

#include "LoggingService.hpp"

#include "ServiceLocator.hpp"

namespace aw {
namespace core {
namespace logging {

void RegisterService(base::ServiceLocatorPtr serviceLocator) {

	if(!serviceLocator) {
		throw std::invalid_argument("Invalid service-locator to register logging service to.");
	}
	
	serviceLocator->RegisterService(ILoggingServicePtr(new LoggingService()));
}

} // namespace logging
} // namespace core
} // namespace aw