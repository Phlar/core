#include "FileLogger.hpp"

#include "Utils.hpp"

#include <boost/thread/lock_guard.hpp>

#include <fstream>

namespace aw {
namespace core {
namespace logging {

namespace {

//! \brief Creates or appends a message to a file.
//! \param fileName Path leading to the file. If file does not exist it will be created.
//! \param message  Text to write to file. In case of NULL nothing will be added, however
//!        the file still will be created if not existing.
//! \param append   Specifies whether to delete the content first or append to the end.
void writeToFile(const boost::filesystem::path& filePath, const char* message, bool append = true) {

    std::ofstream outStream;
    try {

        outStream.open(filePath.string().c_str(), append ? std::ofstream::app : std::ofstream::trunc);
    } catch(const std::exception& e) {

        std::stringstream errorMessage;
        errorMessage << "Exception caught while opening log-file '" << filePath.string() << "': " << e.what();
        throw std::runtime_error(errorMessage.str());

    } catch( ... ) {

        std::stringstream errorMessage;
        errorMessage << "Unknown exception caught while opening log-file '" << filePath.string() << "'";
        throw std::runtime_error(errorMessage.str());
    }

    if(message) {
        outStream << message << std::endl;
    }
}

} // namespace anonymous


FileLogger::FileLogger(const std::string& fileName)
: m_absoluteFilePath(base::utils::GetProcessDirectory() / fileName) {

    // No need to synchronize here...
    writeToFile(m_absoluteFilePath, std::string(120, '*').c_str(), false);
}

FileLogger::~FileLogger() {
}

void FileLogger::Log(const char* message) {

    boost::lock_guard<boost::mutex> guard(m_mutex);
    writeToFile(m_absoluteFilePath, message, true);
}

} // namespace logging
} // namespace core
} // namespace aw