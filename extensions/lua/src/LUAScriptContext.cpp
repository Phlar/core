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
#include <boost/unordered_set.hpp>

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
                                   const std::string& scriptSource)
: m_scriptSource(scriptSource)
, m_luaState(nullptr)
, m_converterFunctions(converterFunctions)
, m_forceGCAfterScriptExecution(defaultForceGCAfterExecution) {

    if(!m_converterFunctions) {
        throw std::invalid_argument("Error creating LUA-context, invalid converter functions provided.");
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
    // Todo: As soon as a script gets executed via an ITask luaL_openlibs()
    // will crash the application immediately. It boils down the crash is
    // caused by calling luaopen_package(). Still no clue, what causes it though,
    // but for now enabling LUA loading packages is disabled...
    // luaL_openlibs(m_luaState);
    luaopen_base(m_luaState);
    luaopen_coroutine(m_luaState);
    luaopen_table(m_luaState);
    luaopen_io(m_luaState);
    luaopen_os(m_luaState);
    luaopen_string(m_luaState);
    luaopen_bit32(m_luaState);
    luaopen_math(m_luaState);
    luaopen_debug(m_luaState);


    // Load and compile the script.
    loadScriptSource();

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

void LUAScriptContext::loadScriptSource() {

    if(luaL_loadstring(m_luaState, m_scriptSource.c_str())) {

        std::stringstream errorMessage;
        errorMessage << "Error loading LUA script-source'"
                     << m_scriptSource << "': "
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
                      << ") was not changed during execution." << std::endl;
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
                         << functionName << "' from source '"
                         << m_scriptSource << "'"
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
        errorMessage << " from source '" << m_scriptSource
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
}

void LUAScriptContext::pushArguments(const ArgumentVector& params) {

    if(params.empty()) {
        return;
    }

    if(m_converterFunctions->toLUAConversionFunctions.empty()) {

        std::stringstream errorMessage;

        errorMessage << "Error pushing arguments to LUA - no argument conversion function(s) registered, following types are provided:" << std::endl;
        boost::unordered_set<std::string> uniqueTypes;
        std::transform(params.cbegin(), params.cend(), std::inserter(uniqueTypes, uniqueTypes.end()), [](const Argument& param){return param.type().name();});
        std::for_each(uniqueTypes.cbegin(), uniqueTypes.cend(), [&errorMessage](const std::string& argType) {errorMessage << argType << std::endl;});

        throw std::runtime_error(std::runtime_error(errorMessage.str()));
    }

    unsigned short paramIndex = 0;
    auto errorMessagePrefixCreator = [&paramIndex]() -> std::stringstream {
        std::stringstream errorMessage;
        errorMessage << "Error converting input-parameter at position " 
                     << paramIndex << ".";
        return errorMessage;
    };

    for(const Argument& arg : params) {

        // ++paramIndex;

        const auto findIter = m_converterFunctions->toLUAConversionFunctions.find(arg.type().hash_code());

        if(findIter == m_converterFunctions->toLUAConversionFunctions.end()) {
            
            // Bad, as there's no functor registered that is capable of pushing the type
            // to the LUA stack.
            std::stringstream errorMessage = errorMessagePrefixCreator();
            errorMessage << " - " << "No LUA converter registered for the provided type '" << arg.type().name() << "'";
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

        ++paramIndex;
    }
}

void LUAScriptContext::fetchReturnValuesFromLUA(const ReturnValuesHolder& results) {

    if(results.Empty()) {
        return;
    }

    if(m_converterFunctions->fromLUAConversionFunctions.empty()) {

        boost::unordered_set<std::string> uniqueTypes;
        for(uint8_t index = 0; index < results.Size(); ++index) {
            uniqueTypes.insert(results.GetValueTypeName(index));
        }

        std::stringstream errorMessage;
        errorMessage << "Error fetching results from LUA - no conversion function(s) registered, following types are provided:" << std::endl;
        std::for_each(uniqueTypes.cbegin(), uniqueTypes.cend(), [&errorMessage](const std::string& argType) {errorMessage << argType << std::endl;});
        throw std::runtime_error(errorMessage.str());
    }

    auto errorMessagePrefixCreator = [](uint8_t index) -> std::stringstream {
        std::stringstream errorMessage;
        errorMessage << "Error converting output-value at position " 
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
            errorMessage << "No LUA converter registered for the provided type '" << retVal.type().name() << "'.";
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