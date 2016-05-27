#pragma once

#include "ILogger.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>


namespace aw {
namespace core {
namespace logging {

class FileLogger : public base::InterfaceImpl<core::logging::ILogger> {

    public:

        //! \brief Specify a file to log to by either providing a file name.
        FileLogger(const std::string& fileName);

        virtual ~FileLogger();

        virtual void Log(const char* message);

    protected:

        // Absolute file-path to the log-file.
        boost::filesystem::path m_absoluteFilePath;

        // Synchronize writes to file.
        boost::mutex m_mutex;
};

} // namespace logging
} // namespace core
} // namespace aw
