#include "InitLoggingService.hpp"

#include "LoggingService.hpp"

#include "ServiceLocator.hpp"

namespace aw {
namespace core {
namespace logging {

ILoggingServicePtr RegisterService(base::ServiceLocatorPtr serviceLocator) {

    if(!serviceLocator) {
        throw std::invalid_argument("Invalid service-locator to register logging service to.");
    }

    ILoggingServicePtr loggingService = ILoggingServicePtr(new LoggingService());
    serviceLocator->RegisterService(loggingService);
    return loggingService;
}

} // namespace logging
} // namespace core
} // namespace aw