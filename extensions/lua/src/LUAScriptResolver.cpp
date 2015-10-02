#include "LUAScriptResolver.hpp"

#include "LUAScriptContext.hpp"


namespace aw {
namespace core {
namespace scripting {
namespace lua {

const char* LUAScriptResolver::m_supportedFileExtension = ".lua";

LUAScriptResolver::LUAScriptResolver() {
}

LUAScriptResolver::~LUAScriptResolver() {
}

bool LUAScriptResolver::IsFileSupported(const boost::filesystem::path& scriptPath) {

    return (scriptPath.extension().string().c_str() == m_supportedFileExtension);
}

IScriptContextPtr LUAScriptResolver::GetContext(const boost::filesystem::path& scriptPath) {

    return IScriptContextPtr(new LUAScriptContext(scriptPath));
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw