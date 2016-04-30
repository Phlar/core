#include "LUAScriptResolver.hpp"

#include "LUAScriptContext.hpp"

#include <boost/make_shared.hpp>

namespace aw {
namespace core {
namespace scripting {
namespace lua {


namespace {

const char* toLUARegistrationErrorString = "Error registering function to pushing to LUA";
const char* fromLUARegistrationErrorString = "Error registering function to fetching from LUA";

template <class Container, class Function>
void registerConverterFunction(const boost::typeindex::type_index& index, const Function& function,
                          Container& container, const char* errorMessage) {

    assert(errorMessage);

    if(!function) {

        std::stringstream errorMessage;
        errorMessage << errorMessage << ": " << "invalid converter function provided.";
        throw std::invalid_argument(errorMessage.str());
    }

    auto insertResult = container.insert(std::make_pair(index, function));
    if(!insertResult.second) {

        std::stringstream errorMessage;
        errorMessage << errorMessage << ": " 
                     << "converter function already registered for type ("
                     << index.pretty_name() << ").";
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

bool LUAScriptResolver::IsFileSupported(const boost::filesystem::path& scriptPath) {

    return (scriptPath.extension().string().c_str() == m_supportedFileExtension);
}

IScriptContextPtr LUAScriptResolver::GetContext(const boost::filesystem::path& scriptPath) {

    return IScriptContextPtr(new LUAScriptContext(m_converterFunctions, scriptPath));
}

void LUAScriptResolver::AddTypeRegistrationFunction(const TypeRegistrationFunction& registrationFunction) {

    if(!registrationFunction) {
        throw std::invalid_argument("Error adding LUA type/function registration function, invalid functor.");
    }

    m_converterFunctions->typeRegistrationFunctions.push_back(registrationFunction);
}

void LUAScriptResolver::RegisterPushToLUAFunction(const boost::typeindex::type_index& regType,
                                                  const PushToLUAFunction& fnc) {

    registerConverterFunction(regType, fnc, m_converterFunctions->toLUAConversionFunctions, toLUARegistrationErrorString);
}

void LUAScriptResolver::RegisterFetchFromLUAFunction(const boost::typeindex::type_index& regType,
                                                    const FetchFromLUAFunction& fnc) {

    registerConverterFunction(regType, fnc, m_converterFunctions->fromLUAConversionFunctions, fromLUARegistrationErrorString);
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw