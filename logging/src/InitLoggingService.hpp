#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace base {

class ServiceLocator;
typedef boost::shared_ptr<ServiceLocator> ServiceLocatorPtr;

} // namespace base

namespace logging {

class ILoggingService;
typedef boost::intrusive_ptr<ILoggingService> ILoggingServicePtr;

ILoggingServicePtr RegisterService(base::ServiceLocatorPtr serviceLocator);

} // namespace logging
} // namespace core
} // namespace aw
