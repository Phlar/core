#pragma once

#include "IService.hpp"
#include "ILogger.hpp"

#include "UUID.hpp"

#include <boost/filesystem.hpp>
#include <boost/intrusive_ptr.hpp>

#include <string>

namespace aw {
namespace core {
namespace logging {


    //! \brief Service storing all installed loggers and dispatches
//! a log call to all of them in order of registration.
class ILoggingService : virtual base::IService {

    public:

        //@{
        //! Implementations of IService
        virtual base::UUID GetServiceID() const = 0;
        //@}

        //! \brief Create and register simple file logger.
        virtual void CreateFileLogger(const boost::filesystem::path& fileName) = 0;

        //! \brief Passes a message to all registered loggers in the order of their construction.
        virtual void Log(LogLevel logLevel, const char* message) = 0;
};
typedef boost::intrusive_ptr<ILoggingService> ILoggingServicePtr;

} // namespace logging
} // namespace core
} // namespace aw