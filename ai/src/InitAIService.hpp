#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace base {

class ServiceLocator;
typedef boost::shared_ptr<ServiceLocator> ServiceLocatorPtr;

} // namespace base

namespace ai {

class IAIService;
typedef boost::intrusive_ptr<IAIService> IAIServicePtr;

IAIServicePtr RegisterService(base::ServiceLocatorPtr serviceLocator);

} // namespace ai
} // namespace core
} // namespace aw
