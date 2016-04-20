#include "LUAScriptContext.hpp"

#pragma warning(disable: 4099)
#include <lua.hpp>
#include <lauxlib.h>
#pragma warning(default: 4099)

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>

#include <iostream>
#include <sstream>

namespace aw {
namespace core {
namespace scripting {
namespace lua {

namespace {

class LUAFunctionParameterVisitor : public boost::static_visitor<void> {

    public:
        LUAFunctionParameterVisitor(lua_State* luaState)
        : m_luaState(luaState) {
        }

        template<class T>
        void operator()(const T& value) {
            luabind::object obj(m_luaState, value);
            obj.push(m_luaState);
        }

    protected:

        lua_State* m_luaState;
};


} // namespace anonymous

const bool LUAScriptContext::defaultForceGCAfterExecution = false;

LUAScriptContext::LUAScriptContext(ConverterFunctionsPtr converterFunctions,
                                   const boost::filesystem::path& scriptPath)
: m_luaState(nullptr)
, m_scriptPath(scriptPath)
, m_converterFunctions(converterFunctions)
, m_forceGCAfterScriptExecution(defaultForceGCAfterExecution) {

    if(!m_converterFunctions) {
        throw std::invalid_argument("Error creating LUA-context, invalid converter functions provided.");
    }

    // Todo: Duplicated somewhere else for sure!
    boost::system::error_code errorCode;
    if(!boost::filesystem::exists(scriptPath, errorCode) || errorCode) {

        std::stringstream errorMessage;
        errorMessage << "Error while checking for LUA script-file '"
            << scriptPath.string() << "'";

        if(errorCode) {
            errorMessage << " (error code: " << errorCode << ")";
        }
        throw std::invalid_argument(errorMessage.str());
    }

    initializeLUAStateObject();
}

LUAScriptContext::~LUAScriptContext() {

    if(m_luaState) {
        lua_close(m_luaState);
    }
}

void LUAScriptContext::ExecuteScript(const std::string& functionName,
                                     const ArgumentVector& params,
                                     const ReturnValuesHolder& results) {

    // Todo: Check whether these cannot be loaded from within LUA code.
    luaL_openlibs(m_luaState);

    // Load and compile the script.
    loadScriptFile();

    // Though the file is loaded and compiled the global table entries
    // (function names, variables) are not known to LUA yet. This does
    // not matter in case of executing the script "from the beginning"
    // however trying to run a specific function using lua_pcall will
    // fail with "attempt to call a nil value". In that case we have 
    // to run the script as it is in order to make the global table
    // aware of the names.
    // (Todo: Check whether this really is the way to go as it obviously
    // has drawbacks (performance, executing anything "free".)
    if(!functionName.empty()) {
        executeScript(std::string(), ArgumentVector(), ReturnValuesHolder::Create());
    }
    
    executeScript(functionName, params, results);
}

void LUAScriptContext::ForceGCAfterExecution(bool forceGC) {

    m_forceGCAfterScriptExecution = forceGC;
}

void LUAScriptContext::initializeLUAStateObject() {

    if(m_luaState) {
        throw std::runtime_error("Error setting up LUA state object, state object already set up.");
    }

    m_luaState = luaL_newstate();
    if(!m_luaState) {
        throw std::runtime_error("Error setting up LUA state object.");
    }

    // Make luabind aware of the state.
    luabind::open(m_luaState);

    // Execute all registration functions.
    for(TypeRegistrationFunction& regFnc : m_converterFunctions->typeRegistrationFunctions) {
        luabind::module(m_luaState)
        [
            regFnc()
        ];
    }
}

void LUAScriptContext::loadScriptFile() {

    if(luaL_loadfile(m_luaState, m_scriptPath.string().c_str())) {

        std::stringstream errorMessage;
        errorMessage << "Error loading LUA script-file '"
                     << m_scriptPath.string() << "': "
                     << lua_tostring(m_luaState, -1);
        lua_pop(m_luaState,1);

        throw std::runtime_error(errorMessage.str());
    }
}

void LUAScriptContext::executeScript(const std::string& functionName, const ArgumentVector& params, const ReturnValuesHolder& results) {

    int16_t numParameters = static_cast<int16_t>(params.size());

    if(functionName.empty()) {

        // Executing the script entirely from top down means
        // not providing any parameters.
        if(!params.empty()) {
            // Todo: At least log this!
            numParameters = 0;
        }
    } else {

        // Resolve the function.
        lua_getglobal(m_luaState, functionName.c_str());

        // If function got resolved properly, the stack's top element
        // now is of type 'LUA_TFUNCTION'.
        const int topType = lua_type(m_luaState, lua_gettop(m_luaState));
        if(topType != LUA_TFUNCTION) {

            std::stringstream errorMessage;
            errorMessage << "Error retrieving function '"
                         << functionName << "' in file '"
                         << m_scriptPath.string() << "'"
                         << " (top of the stack type: "
                         << topType << ")";

            throw std::runtime_error(errorMessage.str());
        }

        // Push all parameters onto the LUA stack.
        pushArguments(params);
    }

    const int result = lua_pcall(m_luaState, numParameters, results.Size(), 0);

    if(m_forceGCAfterScriptExecution) {
        lua_gc(m_luaState, LUA_GCCOLLECT, 0);
    }

    if(result) {

        std::stringstream errorMessage;
        errorMessage << "Error while executing (";
        if(functionName.empty()) {
            errorMessage << "entire script)";
        } else {
            errorMessage << functionName << ")";
        }
        errorMessage << " in file '" << m_scriptPath.string() 
                     << "' (error-code: " << result << ") "
                     << lua_tostring(m_luaState, -1);
        lua_pop(m_luaState,1);

        throw std::runtime_error(errorMessage.str());
    }

    fetchReturnValuesFromLUA(results);


    // Everything seems run well - so check whether there's a return value present.
    std::cout << "*****************************************************************" << std::endl
              << "Number of elements on the stack: " << lua_gettop(m_luaState) << std::endl;

    while (lua_gettop(m_luaState))
    {
        std::cout << "type: " << lua_type(m_luaState, lua_gettop(m_luaState)) << std::endl;
        lua_pop(m_luaState, 1);
    }
    std::cout << "*****************************************************************" << std::endl;
}

void LUAScriptContext::pushArguments(const ArgumentVector& params) {

    if(params.empty()) {
        return;
    }

    if(m_converterFunctions->toLUAConversionFunctions.empty()) {
        throw std::runtime_error("No argument conversion function registered.");
    }

    unsigned short paramIndex = 0;
    auto errorMessagePrefixCreator = [&paramIndex]() -> std::stringstream {
        std::stringstream errorMessage;
        errorMessage << "Error converting parameter at position " 
                     << paramIndex << ".";
        return errorMessage;
    };

    for(const Argument& arg : params) {

        ++paramIndex;
        bool result = false;

        // todo: Replace iterating over a plain container by a lookup.

        const boost::typeindex::type_index paramType(boost::typeindex::type_info<Argument>());

        for(ArgumentConversionFunction& conversionFnc : m_converterFunctions->argumentConversionFunctions) {

            try {
                result = conversionFnc(m_luaState, arg);
                if(result) {
                    break;
                }
            } catch(const std::exception& e) {

                std::stringstream errorMessage = errorMessagePrefixCreator();
                errorMessage << " - " << e.what();
                throw std::runtime_error(errorMessage.str());
            } catch(...) {
                throw std::runtime_error(errorMessagePrefixCreator().str());
            }
        }

        if(!result) {

            // Bad, as there's no functor registered that is capable of pushing the type
            // to the LUA stack.
            std::stringstream errorMessage = errorMessagePrefixCreator();
            errorMessage << " - " << ". No LUA converter registered for the provided type.";
            throw std::runtime_error(errorMessage.str());
        }
    }
}

void LUAScriptContext::fetchReturnValuesFromLUA(const ReturnValuesHolder& /*results*/) {
    

}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw