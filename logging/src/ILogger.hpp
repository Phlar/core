#pragma once

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace logging {


enum class LogLevel {
    LOG_DEBUG = 0,
    LOG_WARNING,
    LOG_ERROR,
    LOG_INFO
};

class ILogger : public virtual base::IReferenceCounted {

    public:

         virtual void Log(const char* message) = 0;
};
typedef boost::intrusive_ptr<ILogger> ILoggerPtr;

} // namespace scripting
} // namespace core
} // namespace aw
