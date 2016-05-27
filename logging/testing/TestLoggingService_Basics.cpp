#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_LoggingServiceBasics"

#include "ServiceLocator.hpp"
#include "ILoggingService.hpp"

#include "InitLoggingService.hpp"

#include "Utils.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#pragma warning(disable: 4512)
#include <boost/scope_exit.hpp>
#pragma warning(default: 4512)


namespace aw {
namespace core {
namespace logging {
namespace testing {

namespace {

struct LoggingTestFixture {

    LoggingTestFixture()
    : serviceLocator(base::ServiceLocatorPtr())
    , loggingService(ILoggingServicePtr()) {

        BOOST_REQUIRE_NO_THROW(serviceLocator = base::ServiceLocator::Instance());
        BOOST_REQUIRE(serviceLocator);

        BOOST_REQUIRE_NO_THROW(RegisterService(serviceLocator));

        // Check if service is available...
        base::IServicePtr service = base::IServicePtr();
        BOOST_REQUIRE_NO_THROW(service = serviceLocator->GetService(ID_LOGGING_SERVICE));
        BOOST_REQUIRE(service);

        // ...and the right one...
        loggingService = boost::dynamic_pointer_cast<ILoggingService>(service);
        BOOST_REQUIRE(loggingService);
    }

    base::ServiceLocatorPtr serviceLocator;
    ILoggingServicePtr loggingService;
};

} // namespace anonymous


BOOST_FIXTURE_TEST_CASE(TestCreateFileLogger, LoggingTestFixture) {

    const std::string fileName("log.txt");
    const boost::filesystem::path absoluteFilePath = base::utils::GetProcessDirectory() / fileName;

#pragma warning(disable: 4003 4512)
    // Ensure the file got deleted in case of an error.
    BOOST_SCOPE_EXIT(&absoluteFilePath) {

        bool fileExists = false;
        BOOST_REQUIRE_NO_THROW(fileExists = boost::filesystem::exists(absoluteFilePath));

        if(fileExists) {

            BOOST_REQUIRE_NO_THROW(boost::filesystem::remove(absoluteFilePath));
        }

    } BOOST_SCOPE_EXIT_END
#pragma warning(default: 4003 4512)

    // File name only.
    bool fileExists = false;
    BOOST_REQUIRE_NO_THROW(fileExists = boost::filesystem::exists(absoluteFilePath));
    BOOST_CHECK(!fileExists);
    if(fileExists) {
        BOOST_REQUIRE_NO_THROW(boost::filesystem::remove(absoluteFilePath));
    }

    // Ensure the file's not there
    BOOST_REQUIRE_NO_THROW(loggingService->CreateFileLogger(fileName));
    BOOST_REQUIRE_NO_THROW(fileExists = boost::filesystem::exists(absoluteFilePath));
    BOOST_CHECK(fileExists);

    uintmax_t sizeBefore = 0;
    BOOST_REQUIRE_NO_THROW(sizeBefore = boost::filesystem::file_size(absoluteFilePath));

    // Write some lines...
    loggingService->Log(LogLevel::LOG_DEBUG, "Helping debug message.");
    loggingService->Log(LogLevel::LOG_INFO,  "Just informing someone.");
    loggingService->Log(LogLevel::LOG_WARNING, "Not that good anymore.");
    loggingService->Log(LogLevel::LOG_ERROR, "Ooops.");

    uintmax_t sizeAfter = 0;
    BOOST_REQUIRE_NO_THROW(sizeAfter = boost::filesystem::file_size(absoluteFilePath));

    BOOST_CHECK(sizeAfter > sizeBefore);
}


} // namespace testing
} // namespace logging
} // namespace core
} // namespace aw