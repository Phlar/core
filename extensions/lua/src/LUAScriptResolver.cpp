#include "LUAScriptResolver.hpp"


namespace aw {
namespace core {
namespace scripting {
namespace lua {

LUAScriptResolver::LUAScriptResolver() {
}

LUAScriptResolver::~LUAScriptResolver() {
}

bool LUAScriptResolver::IsFileSupported(const boost::filesystem::path& /*scriptPath*/) {

    return true;
}

IScriptContextPtr LUAScriptResolver::GetContext(const boost::filesystem::path& /*scriptPath*/) {

    return IScriptContextPtr();
}

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw