#pragma once

#include <luabind/scope.hpp>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

namespace aw {
namespace core {
namespace scripting {
namespace lua {

typedef boost::function<luabind::scope()> RegistrationFunction;
typedef std::vector<RegistrationFunction> RegistrationFunctions;
typedef boost::shared_ptr<RegistrationFunctions> RegistrationFunctionsPtr;


} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw