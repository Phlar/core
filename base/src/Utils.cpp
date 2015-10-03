#include "Utils.hpp"

#include <sstream>

namespace aw {
namespace core {
namespace base {
namespace utils {

boost::filesystem::path GetProcessFilePath() {

    HMODULE moduleHandle = GetModuleHandle(nullptr);
    if(!moduleHandle) {
        throw std::runtime_error("Error retrieving module handle.");
    }

    char moduleName[MAX_PATH];
    size_t moduleNameLength = 0;
    moduleNameLength = GetModuleFileName(moduleHandle, moduleName, sizeof(moduleName));
    if(moduleNameLength == 0) {
        throw std::runtime_error("Error retrieving module name.");
    }

    return boost::filesystem::path(std::string(moduleName, moduleNameLength));
}

boost::filesystem::path GetProcessDirectory() {

    boost::filesystem::path processDirectory(GetProcessFilePath().parent_path());

    std::stringstream errorMessage;
    boost::system::error_code errorCode;
    if(!boost::filesystem::is_directory(processDirectory, errorCode)) {
        errorMessage << "Error checking directory '" << processDirectory << "'";
        throw std::runtime_error(errorMessage.str());
    }

    if(errorCode) {
        errorMessage << "Error while checking directory '" << processDirectory
                     << "' (error-code: " << errorCode << ").";
        throw std::runtime_error(errorMessage.str());
    }
    return processDirectory;
}


} // namespace utils
} // namespace base
} // namespace core
} // namespace aw