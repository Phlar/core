#include "LUAScriptResolver.hpp"

#include "LUAScriptContext.hpp"

#include <boost/make_shared.hpp>

namespace aw {
namespace core {
namespace scripting {
namespace lua {

const char* LUAScriptResolver::m_supportedFileExtension = ".lua";

LUAScriptResolver::LUAScriptResolver()
: m_registrationFunctions(boost::make_shared<RegistrationFunctions>()) {
}

LUAScriptResolver::~LUAScriptResolver() {
}

bool LUAScriptResolver::IsFileSupported(const boost::filesystem::path& scriptPath) {

    return (scriptPath.extension().string().c_str() == m_supportedFileExtension);
}

IScriptContextPtr LUAScriptResolver::GetContext(const boost::filesystem::path& scriptPath) {

    return IScriptContextPtr(new LUAScriptContext(m_registrationFunctions, scriptPath));
}

void LUAScriptResolver::AddRegistrationFunction(const RegistrationFunction& registrationFunction) {

    if(!registrationFunction) {
        throw std::invalid_argument("Error adding LUA registration function, invalid functor.");
    }

    m_registrationFunctions->push_back(registrationFunction);
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw