#include "LUAScriptContext.hpp"

#pragma warning(disable: 4099)
#include <lua.hpp>
#include <lauxlib.h>
#pragma warning(default: 4099)

#pragma warning(disable: 4512)
#include <boost/scope_exit.hpp>
#pragma warning(default: 4512)

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

const bool LUAScriptContext::defaultForceGCAfterExecution = true;

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
    int16_t numReturnValues = static_cast<int16_t>(results.Size());
    const bool runEntireFile = functionName.empty();
    const int stackSizeBefore = lua_gettop(m_luaState);


#pragma warning(disable: 4003 4512)
    // Ensure LUA stack is of same size as before after execution or in case of an abort.
    BOOST_SCOPE_EXIT(&stackSizeBefore, this_) {

        if(!this_->m_luaState) {
            std::cout << "Cannot clean up LUA stack - invalid LUA state object." << std::endl;
            return;
        }

        // todo: Debug log!
        const int stackSizeNow = lua_gettop(this_->m_luaState);
        const int elementsToPop = stackSizeNow - stackSizeBefore;

        if(elementsToPop < 0) {
            std::cout << "Error cleaning LUA stack. Less elements on stack (" << stackSizeNow
                      << ") than before (" << stackSizeBefore << ") - possible stack corruption." << std::endl;
        } else if(elementsToPop == 0) {
            std::cout << "No need to clean LUA stack as its stack count (" << stackSizeNow 
                      << ") was not changed during exectution." << std::endl;
        } else {

            lua_pop(this_->m_luaState, elementsToPop);
            std::cout << "Cleaning up LUA stack of " << elementsToPop << " elements. (before: " 
                      << stackSizeBefore << ", now: " << lua_gettop(this_->m_luaState) << ")" << std::endl;
        }

        if(this_->m_forceGCAfterScriptExecution) {
            lua_gc(this_->m_luaState, LUA_GCCOLLECT, 0);
        }

    } BOOST_SCOPE_EXIT_END
#pragma warning(default: 4003 4512)

    if(runEntireFile) {

        // Executing the script entirely from top down means
        // not providing any parameters.
        if(!params.empty()) {
            // Todo: At least log this!
            numParameters = 0;
            numReturnValues = 0;
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

    const int result = lua_pcall(m_luaState, numParameters, LUA_MULTRET, 0);
    const int stackSizeAfter = lua_gettop(m_luaState);

    // Error during execution, e.g. function could not be resolved.
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

    // Number of returned elements mismatch.
    if(!runEntireFile && ((stackSizeAfter - stackSizeBefore) != numReturnValues)) {

        std::stringstream errorMessage;
        errorMessage << "Error while executing (" << functionName << ") - expected "
                     << numReturnValues << " return values, got " << stackSizeAfter << ".";

        throw std::runtime_error(errorMessage.str());
    }

    // Retrieve expected return values from LUA - in case of a function was called.
    // todo: Clarify if that's true - can "execution of a file" return something?
    if(!runEntireFile) {
        fetchReturnValuesFromLUA(results);
    }

    // todo: Check whether LUA stack is the same as before execution?
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

        const auto findIter = m_converterFunctions->toLUAConversionFunctions.find(arg.type().hash_code());

        if(findIter == m_converterFunctions->toLUAConversionFunctions.end()) {
            
            // Bad, as there's no functor registered that is capable of pushing the type
            // to the LUA stack.
            std::stringstream errorMessage = errorMessagePrefixCreator();
            errorMessage << " - " << ". No LUA converter registered for the provided type.";
            throw std::runtime_error(errorMessage.str());
        }

        bool conversionResult = false;
        try {

            // Though the lookup claims the converter should be correct, the converter itself knows best...
            conversionResult = findIter->second(m_luaState, arg);

        } catch(const std::exception& e) {

            std::stringstream errorMessage = errorMessagePrefixCreator();
            errorMessage << " - " << e.what();
            throw std::runtime_error(errorMessage.str());
        } catch(...) {
            throw std::runtime_error(errorMessagePrefixCreator().str());
        }

        if(!conversionResult) {
            std::stringstream errorMessage = errorMessagePrefixCreator();
            errorMessage << " Converter found for type [" << arg.type().name() << "] however conversion function returned an error";
            throw std::runtime_error(errorMessage.str());
        }
    }
}

void LUAScriptContext::fetchReturnValuesFromLUA(const ReturnValuesHolder& results) {

    if(results.Empty()) {
        return;
    }

    if(m_converterFunctions->fromLUAConversionFunctions.empty()) {
        throw std::runtime_error("No argument conversion function registered.");
    }

    auto errorMessagePrefixCreator = [](uint8_t index) -> std::stringstream {
        std::stringstream errorMessage;
        errorMessage << "Error converting value at position " 
                     << index << ".";
        return errorMessage;
    };

    for(uint8_t i=results.Size(); i--;) {

        const ReturnValue& retVal = results.GetValue(i);

        const auto findIter = m_converterFunctions->fromLUAConversionFunctions.find(retVal.type().hash_code());

        if(findIter == m_converterFunctions->fromLUAConversionFunctions.end()) {

            // Bad, as there's no functor registered that is capable of poping the type
            // from the LUA stack.
            std::stringstream errorMessage;
            errorMessage << "No LUA converter registered for the provided type ["
                         << retVal.type().name() << "].";
            throw std::runtime_error(errorMessage.str());
        }

        try {

            results.SetValue(i, findIter->second(m_luaState));

        } catch(const std::exception& e) {

            std::stringstream errorMessage = errorMessagePrefixCreator(i);
            errorMessage << " - " << e.what();
            throw std::runtime_error(errorMessage.str());
        } catch(...) {
            throw std::runtime_error(errorMessagePrefixCreator(i).str());
        }
    }
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw