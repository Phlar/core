#include "LUAScriptResolver.hpp"

#include "LUAScriptContext.hpp"

#include <boost/make_shared.hpp>

#include <iostream>

namespace aw {
namespace core {
namespace scripting {
namespace lua {


namespace {

const char* toLUARegistrationErrorString = "Error registering function to pushing to LUA";
const char* fromLUARegistrationErrorString = "Error registering function to fetching from LUA";

template <class Container, class Function>
void registerTypeConverterFunction(const boost::typeindex::type_info& typeInfo, const Function& function,
                                   Container& container, const char* errorMessage) {

    assert(errorMessage);

    if(!function) {

        std::stringstream errorMessage;
        errorMessage << errorMessage << ": " << "invalid converter function provided.";
        throw std::invalid_argument(errorMessage.str());
    }

    auto insertResult = container.insert(std::make_pair(typeInfo.hash_code(), function));
    if(!insertResult.second) {

        std::stringstream errorMessage;
        errorMessage << errorMessage << ": " 
                     << "converter function already registered for type ("
                     << typeInfo.name() << ").";
        throw std::runtime_error(errorMessage.str());
    }
}

} // namespace anonymous

const char* LUAScriptResolver::m_supportedFileExtension = ".lua";

LUAScriptResolver::LUAScriptResolver()
: m_converterFunctions(boost::make_shared<ConverterFunctions>()) {
}

LUAScriptResolver::~LUAScriptResolver() {
}

base::UUID LUAScriptResolver::GetResolverID() const {

    return ID_LUA_SCRIPT_RESOLVER;
}

bool LUAScriptResolver::IsFileSupported(const boost::filesystem::path& scriptPath) {

    return (scriptPath.extension().string().c_str() == m_supportedFileExtension);
}

IScriptContextPtr LUAScriptResolver::GetContextFromFile(const boost::filesystem::path& filePath) {

    std::string scriptSource;

    boost::system::error_code errorCode;
    if(!boost::filesystem::exists(filePath, errorCode) || errorCode) {

        std::stringstream errorMessage;
        errorMessage << "Error while checking LUA script-file '"
            << filePath.string() << "'";

        if(errorCode) {
            errorMessage << " (error code: " << errorCode << ")";
        }
        throw std::invalid_argument(errorMessage.str());
    }

    try {

        std::ifstream file(filePath.string().c_str());
        if(!file) {
            throw std::runtime_error("Invalid file stream.");
        }

        std::stringstream strStream;
        strStream << file.rdbuf();
        scriptSource = strStream.str();

    } catch(const std::exception& e) {

        std::stringstream errorMessage;
        errorMessage << "Exception caught while reading LUA script-file '"
            << filePath.string() << "': " << e.what();
        throw std::runtime_error(errorMessage.str());

    } catch(...) {

        std::stringstream errorMessage;
        errorMessage << "Unknown exception caught while reading LUA script-file '" << filePath.string();
        throw std::runtime_error(errorMessage.str());
    }

    return IScriptContextPtr(new LUAScriptContext(m_converterFunctions, scriptSource));
}

IScriptContextPtr LUAScriptResolver::GetContextFromString(const std::string& scriptSource) {

    return IScriptContextPtr(new LUAScriptContext(m_converterFunctions, scriptSource));
}

void LUAScriptResolver::AddTypeRegistrationFunction(const TypeRegistrationFunction& registrationFunction) {

    if(!registrationFunction) {
        throw std::invalid_argument("Error adding LUA type/function registration function, invalid functor.");
    }

    m_converterFunctions->typeRegistrationFunctions.push_back(registrationFunction);
}

void LUAScriptResolver::RegisterPushTypeToLUAFunction(const boost::typeindex::type_info& regType,
                                                      const PushToLUAFunction& fnc) {

    registerTypeConverterFunction(regType, fnc, m_converterFunctions->toLUAConversionFunctions, toLUARegistrationErrorString);
}

void LUAScriptResolver::RegisterFetchTypeFromLUAFunction(const boost::typeindex::type_info& regType,
                                                         const FetchFromLUAFunction& fnc) {

    registerTypeConverterFunction(regType, fnc, m_converterFunctions->fromLUAConversionFunctions, fromLUARegistrationErrorString);
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw