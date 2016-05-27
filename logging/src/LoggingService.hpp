#pragma once

#include "ILoggingService.hpp"
#include "ILogger.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>

#include <list>

namespace aw {
namespace core {
namespace logging {

//! \brief Implementation of ILoggingService.
class LoggingService : public base::InterfaceImpl<core::logging::ILoggingService> {

    public:

        LoggingService();

        virtual ~LoggingService();

        //@{
        //! Implementations of IService.
        virtual base::UUID GetServiceID() const;
        //@}

        virtual void CreateFileLogger(const std::string& fileName);

        virtual void Log(LogLevel logLevel, const char* message);

    protected:

        // All registered interpreters.
        std::list<ILoggerPtr> m_loggers;

        // Readable names of all log-levels.
        boost::unordered_map<LogLevel, const char*> m_readableLogLevel;
};

} // namespace logging
} // namespace core
} // namespace aw



