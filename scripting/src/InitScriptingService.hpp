#pragma once

#include <boost/shared_ptr.hpp>

namespace aw {
namespace core {
namespace base {

class ServiceLocator;
typedef boost::shared_ptr<ServiceLocator> ServiceLocatorPtr;

} // namespace base

namespace scripting {

void RegisterService(base::ServiceLocatorPtr serviceLocator);

} // namespace scripting
} // namespace core
} // namespace aw
