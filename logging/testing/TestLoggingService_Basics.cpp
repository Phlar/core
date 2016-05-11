#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_LoggingServiceBasics"

#include "ServiceLocator.hpp"
#include "ILoggingService.hpp"

#include "Utils.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>


namespace aw {
namespace core {
namespace logging {
namespace testing {

namespace {

struct LoggingTestFixture {

    LoggingTestFixture() {

        //BOOST_REQUIRE_NO_THROW(serviceLocator.);
    }

    // base::ServiceLocator serviceLocator;
};

} // namespace anonymous

BOOST_FIXTURE_TEST_CASE(TestCreateFileLogger, LoggingTestFixture) {

    // Check there's no log-file present.
    // Create it without exception.
    // Check presence of file.
    // Log something to it - check size afterwards.

    const std::string fileName("log.txt");
    const boost::filesystem::path absoluteLogFilePath(base::utils::GetProcessDirectory() / fileName);

    // File name only.
    // BOOST_REQUIRE_NO_THROW(!!boost::filesystem::exists(absoluteLogFilePath));



    
    /*
    checkLogFileCreation(boost::filesystem::path("logFileNameOnly.txt"));
    checkLogFileCreation(boost::filesystem::path(base::utils::GetProcessDirectory() / "logFileAbsolutePath.txt"));
    */
}


} // namespace testing
} // namespace logging
} // namespace core
} // namespace aw