#pragma once

#include <boost/shared_ptr.hpp>

namespace aw {
namespace core {
namespace base {

class ServiceLocator;
typedef boost::shared_ptr<ServiceLocator> ServiceLocatorPtr;

} // namespace base

namespace ai {

void RegisterService(base::ServiceLocatorPtr serviceLocator);

} // namespace ai
} // namespace core
} // namespace aw
