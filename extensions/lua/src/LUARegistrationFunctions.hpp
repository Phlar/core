#pragma once

#include <luabind/scope.hpp>

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

namespace aw {
namespace core {
namespace scripting {
namespace lua {

// Typedefs for functions that are used in order to expose C++ classes / functions to LUA,
// as well as typedefs for converting "any" parameters to luabind::object types.
typedef boost::function<luabind::scope()> TypeRegistrationFunction;
typedef std::vector<TypeRegistrationFunction> TypeRegistrationFunctions;

typedef boost::function<bool(lua_State*, const boost::any&)> ArgumentConversionFunction;
typedef std::vector<ArgumentConversionFunction> ArgumentConversionFunctions;

struct ConverterFunctions {

    TypeRegistrationFunctions typeRegistrationFunctions;
    ArgumentConversionFunctions argumentConversionFunctions;
};
typedef boost::shared_ptr<ConverterFunctions> ConverterFunctionsPtr;

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw