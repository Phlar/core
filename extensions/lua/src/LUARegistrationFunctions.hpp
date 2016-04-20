#pragma once

#include <luabind/scope.hpp>

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <vector>

namespace aw {
namespace core {
namespace scripting {
namespace lua {

// Typedefs for functions that are used in order to expose C++ classes / functions to LUA,
// as well as typedefs for converting "any" parameters to luabind::object types.

// todo: rename to "RegisterTypeInLUAFunction"
typedef boost::function<luabind::scope()> TypeRegistrationFunction;
typedef std::vector<TypeRegistrationFunction> TypeRegistrationFunctions;

// todo: rename to "PushToLUAFunction"
typedef boost::function<bool(lua_State*, const boost::any&)> PushToLUAFunction;
typedef boost::unordered_map<boost::typeindex::type_index, PushToLUAFunction> PushToLUAFunctions;

typedef boost::function<bool(lua_State*, const boost::any&)> ArgumentConversionFunction;
typedef std::vector<ArgumentConversionFunction> ArgumentConversionFunctions;

// todo: rename to "FetchFromLUAFunction"
typedef boost::function<boost::any(lua_State*)> FetchFromLUAFunction;
typedef boost::unordered_map<boost::typeindex::type_index, FetchFromLUAFunction> FetchFromLUAFunctions;

typedef boost::function<boost::any(lua_State*)> FromLUAConversionFunction;
typedef boost::unordered_map<boost::typeindex::type_info, FromLUAConversionFunction> FromLUAConversionFunctionsMap;

//! \todo Instead of a plain vector use a map instead for all registered type-conversion functions.
struct ConverterFunctions {

    TypeRegistrationFunctions typeRegistrationFunctions;

    /*
    ArgumentConversionFunctions argumentConversionFunctions;
    FromLUAConversionFunctionsMap fromLUAConversionFunctions;
    */

    PushToLUAFunctions      toLUAConversionFunctions;
    FetchFromLUAFunctions   fromLUAConversionFunctions;
};
typedef boost::shared_ptr<ConverterFunctions> ConverterFunctionsPtr;

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw