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
// as well as typedefs for converting "any" parameters to luabind::object types and back to "any" again.
typedef boost::function<luabind::scope()> TypeRegistrationFunction;
typedef std::vector<TypeRegistrationFunction> TypeRegistrationFunctions;

typedef size_t TypeHash;

typedef boost::function<bool(lua_State*, const boost::any&)> PushToLUAFunction;
typedef boost::unordered_map<TypeHash, PushToLUAFunction> PushToLUAFunctions;

typedef boost::function<boost::any(lua_State*)> FetchFromLUAFunction;
typedef boost::unordered_map<TypeHash, FetchFromLUAFunction> FetchFromLUAFunctions;

struct ConverterFunctions {

    // Container of functors that register all types to LUA - i.e. make the visible to the LUA context.
    TypeRegistrationFunctions typeRegistrationFunctions;

    // Containers of functors "converting" values to / from LUA.
    PushToLUAFunctions      toLUAConversionFunctions;
    FetchFromLUAFunctions   fromLUAConversionFunctions;
};
typedef boost::shared_ptr<ConverterFunctions> ConverterFunctionsPtr;

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw