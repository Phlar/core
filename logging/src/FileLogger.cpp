#include "FileLogger.hpp"

#include <iostream>

namespace aw {
namespace core {
namespace logging {

namespace {

//! \brief Creates or appends a message to a file.
//! \param fileName Path leading to the file. If file does not exist it will be created.
//! \param message  Text to write to file. In case of NULL nothing will be added, however
//!        the file still will be created if not existing.
//! \param append   Specifies whether to delete the content first or append to the end.
void writeToFile(const boost::filesystem::path& /*fileName*/, const char* /*message*/, bool /*append = true*/) {

    

}

} // namespace anonymous


FileLogger::FileLogger(const boost::filesystem::path& /*fileName*/)
: m_absoluteFilePath() {

//    if(boost::filesystem::path::)


}

FileLogger::~FileLogger() {
}

void FileLogger::Log(const char* /*message*/) {


}

} // namespace logging
} // namespace core
} // namespace aw