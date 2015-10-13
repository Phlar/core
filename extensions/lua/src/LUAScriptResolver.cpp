#include "LUAScriptResolver.hpp"

#include "LUAScriptContext.hpp"

#include <boost/make_shared.hpp>

namespace aw {
namespace core {
namespace scripting {
namespace lua {

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

void LUAScriptResolver::AddParameterConverterFunction(const ArgumentConversionFunction& converterFunction) {

    if(!converterFunction) {
        throw std::invalid_argument("Error adding LUA parameter conversion function, invalid functor.");
    }

    m_converterFunctions->argumentConversionFunctions.push_back(converterFunction);
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw