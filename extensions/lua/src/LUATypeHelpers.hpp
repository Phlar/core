#pragma once

#include "LUAScriptResolver.hpp"
#include "Arguments.hpp"

#include <boost/any.hpp>

#include <luabind/scope.hpp>


namespace aw {
namespace core {
namespace scripting {
namespace lua {


//! \brief Helper to push arbitrary types onto the LUA stack.
//! Based on the passed "any" parameter this checks whether it
//! is the expected type. If so that type gets pushed onto the
//! LUA stack.
template<class T>
bool pushToLUAStack(lua_State* luaState, const boost::any& argument) {

    if(!luaState) {
        throw std::invalid_argument("Error using invalid LUA state object when pushing an argument to the function stack.");
    }

    if(argument.empty()) {
        throw std::invalid_argument("Error when trying to push an invalid argument onto LUA function stack.");
    }

    const T* val = boost::any_cast<T>(&argument);
    if(!val) {
        return false;
    }

    try {
        luabind::object toPush(luaState, *val);
        toPush.push(luaState);
    } catch(const std::exception& e) {

        const std::string errorMessage = std::string("Exception caught while pushing argument to LUA stack: ") + std::string(e.what());
        throw std::runtime_error(errorMessage);

    } catch(...) {

        throw std::runtime_error("Unknown exception caught when pushing argument to LUA stack.");
    }
    return true;
}

//! \brief Helper to take arbitrary types from the LUA stack.
//! After the element is taken, the stack size will be decreased by 1.
template <class T>
boost::any popFromLUAStack(lua_State* luaState) {

    if(!luaState) {
        throw std::invalid_argument("Error using invalid LUA state object when poping an element from LUA stack.");
    }

    // We must have at least one element on the stack.
    if(lua_gettop(luaState) < 1) {
        throw std::runtime_error("Error when trying to fetch element from LUA stack - stack must have at least one element.");
    }

    try {

        // Get the topmost element...
        luabind::object stackObject(luabind::from_stack(luaState, -1));
        // ...and remove it from the stack.
        lua_pop(luaState, 1);

        return luabind::object_cast<T>(stackObject);

    } catch(const std::exception& e) {

        const std::string errorMessage = std::string("Exception caught while fetching argument from LUA stack: ") + std::string(e.what());
        throw std::runtime_error(errorMessage);

    } catch(...) {
        throw std::runtime_error("Unknown exception caught when fetching element from LUA stack.");
    }

    throw std::runtime_error("Should not reach this.");
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw