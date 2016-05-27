#include "LoggingService.hpp"

#include "FileLogger.hpp"

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/uuid/string_generator.hpp>

#include <sstream>

namespace aw {
namespace core {
namespace logging {


// Beauty of the initializer list due to ambiguity, see here:
// http://stackoverflow.com/questions/16211410/ambiguity-when-using-boostassignlist-of-to-construct-a-stdvector
LoggingService::LoggingService()
: m_loggers()
, m_readableLogLevel(boost::assign::map_list_of
    (LogLevel::LOG_DEBUG,   "Debug")
    (LogLevel::LOG_WARNING, "Warning ")
    (LogLevel::LOG_ERROR,   "Error   ")
    (LogLevel::LOG_INFO,    "Info    ").convert_to_container<boost::unordered_map<LogLevel, const char*>>()) {
}

LoggingService::~LoggingService() {
}

base::UUID LoggingService::GetServiceID() const {

    return ID_LOGGING_SERVICE;
}

void LoggingService::CreateFileLogger(const std::string& fileName) {

    m_loggers.push_back(ILoggerPtr(new FileLogger(fileName)));
}

void LoggingService::Log(LogLevel logLevel, const char* message) {

    if(m_loggers.empty()) {
        return;
    }

    // Build up the logging message like:
    // Log-level [ThreadID] 12:23:012 Actual message.

    // Passing '0' to the facet's constructor means the locale itself will take care
    // about destruction. http://stackoverflow.com/questions/5330459/ownership-deleteing-the-facet-in-a-locale-stdlocale
    boost::posix_time::time_facet *timeFacet = new boost::posix_time::time_facet();
    timeFacet->format("%d-%b-%Y %H:%M:%S");

    std::stringstream logLine;
    logLine.imbue(std::locale(logLine.getloc(), timeFacet));

    logLine << m_readableLogLevel[logLevel] << "  "
            << "[" << boost::this_thread::get_id() << "]" << "  "
            << boost::posix_time::second_clock::local_time() << "    "
            << message;

    const std::string& strLine = logLine.str();

    for(auto logger : m_loggers) {

        try {
            logger->Log(strLine.c_str());
        } catch(const std::exception& e) {

            std::cerr << "Exception caught while logging message (" 
                      << strLine << "), error: " << e.what() << std::endl;
        } catch( ... ) {

            std::cerr << "Unknown exception caught while logging message (" << strLine << ")" << std::endl;
        }
    }
}

} // namespace logging
} // namespace core
} // namespace aw