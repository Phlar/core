#pragma once

#include "ILogger.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>


namespace aw {
namespace core {
namespace logging {

class FileLogger : public base::InterfaceImpl<core::logging::ILogger> {

    public:

        //! \brief Specify a file to log to by either providing a file name only or
        //! an entire absolute path of the log file. In case of file name only
        //! the directory the file will be put to is the preant process' directory.
        FileLogger(const boost::filesystem::path& fileName);

        virtual ~FileLogger();

        virtual void Log(const char* message);

    protected:

        // Absolute file-path to the log-file.
        boost::filesystem::path m_absoluteFilePath;
};

} // namespace scripting
} // namespace core
} // namespace aw
