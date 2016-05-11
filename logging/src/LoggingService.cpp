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


const base::UUID LoggingService::serviceID = boost::uuids::string_generator()("{81BAA624-EA55-4E98-A73D-4F8883103970}");

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

    return serviceID;
}

void LoggingService::CreateFileLogger(const boost::filesystem::path& fileName) {

    m_loggers.push_back(ILoggerPtr(new FileLogger(fileName)));
}

void LoggingService::Log(LogLevel logLevel, const char* message) {

    if(m_loggers.empty()) {
        return;
    }

    // Build up the logging message like:
    // Log-level [ThreadID] 12:23:012 Actual message.

    static boost::posix_time::time_facet *timeFacet = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");

    std::stringstream logLine;
    logLine.imbue(std::locale(logLine.getloc(), timeFacet));

    logLine << m_readableLogLevel[logLevel] 
            << "[" << boost::this_thread::get_id() << "]"
            << boost::posix_time::second_clock::local_time()
            << message;

    const char* cMessage = logLine.str().c_str();

    for(auto logger : m_loggers) {

        try {
            logger->Log(cMessage);
        } catch(const std::exception& e) {

            std::cerr << "Exception caught while logging message (" 
                      << cMessage << "), error: " << e.what() << std::endl;
        } catch( ... ) {

            std::cerr << "Unknown exception caught while logging message (" << cMessage << ")" << std::endl;
        }
    }
}

} // namespace logging
} // namespace core
} // namespace aw