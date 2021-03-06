#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_ScriptingServiceBasics"

#include <boost/test/unit_test.hpp>

#include "Utils.hpp"

#include "ScriptingService.hpp"

#include "Mock_ScriptContext.hpp"
#include "Mock_ScriptResolver.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace scripting {
namespace testing {

namespace {

struct ScopedFile {

        ScopedFile(const boost::filesystem::path& filePath)
        : scopedFilePath(filePath) {

            if(boost::filesystem::exists(filePath)) {
                BOOST_REQUIRE_NO_THROW(boost::filesystem::remove(scopedFilePath));
            }else {
                std::ofstream file(filePath.string().c_str());
                BOOST_REQUIRE(file);
            }
        }

        virtual ~ScopedFile() {

            BOOST_REQUIRE(boost::filesystem::exists(scopedFilePath));
            BOOST_REQUIRE_NO_THROW(boost::filesystem::remove(scopedFilePath));
        }

    protected:

        boost::filesystem::path scopedFilePath;
};

} // namespace anonymous


struct ScriptingFixture {

    public:

        ScriptingFixture() {

            mockResolverID = base::utils::CreateUUIDFromString("{11111111-AAAA-BBBB-CCCC-DDDDDDDDDDDD}");

            mockScriptResolver = MockScriptResolverPtr(new MockScriptResolver());
            MOCK_EXPECT(mockScriptResolver->GetResolverID).returns(mockResolverID);
        }

        ScriptingService scriptingService;

        typedef boost::intrusive_ptr<MockScriptResolver> MockScriptResolverPtr;
        MockScriptResolverPtr mockScriptResolver;

        base::UUID mockResolverID;
};


BOOST_FIXTURE_TEST_CASE(TestRegisteringResolver, ScriptingFixture) {

    // Registry does not contain any resolver yet.
    BOOST_CHECK_NO_THROW(scriptingService.AddResolver(mockScriptResolver));
}

BOOST_FIXTURE_TEST_CASE(TestRegisteringInvalidResolverShouldThrow, ScriptingFixture) {

    IScriptResolverPtr invalidResolver;
    BOOST_CHECK_THROW(scriptingService.AddResolver(invalidResolver), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(TestRegisteringResolverTwiceShouldThrow, ScriptingFixture) {

    BOOST_CHECK_NO_THROW(scriptingService.AddResolver(mockScriptResolver));
    BOOST_CHECK_THROW(scriptingService.AddResolver(mockScriptResolver), std::logic_error);
}

BOOST_FIXTURE_TEST_CASE(TestContextRetrieval, ScriptingFixture) {

    BOOST_CHECK_NO_THROW(scriptingService.AddResolver(mockScriptResolver));

    const boost::filesystem::path scriptPath("SomeScriptFile.txt");
    ScopedFile scriptFile(scriptPath);

    MOCK_EXPECT(mockScriptResolver->IsFileSupported).once().returns(true);
    MOCK_EXPECT(mockScriptResolver->GetContextFromFile).once().returns(MockScriptContextPtr(new MockScriptContext()));
    
    IScriptContextPtr context = scriptingService.GetContextFromFile(scriptPath);
    BOOST_CHECK(context);
}

BOOST_FIXTURE_TEST_CASE(TestContextRetrievalNoValidResolverFound, ScriptingFixture) {

    BOOST_CHECK_NO_THROW(scriptingService.AddResolver(mockScriptResolver));

    const boost::filesystem::path scriptPath("SomeScriptFile.txt");
    ScopedFile scriptFile(scriptPath);

    MOCK_EXPECT(mockScriptResolver->IsFileSupported).once().returns(false);
    MOCK_EXPECT(mockScriptResolver->GetContextFromFile).never();

    IScriptContextPtr context = scriptingService.GetContextFromFile(scriptPath);
    BOOST_CHECK(!context);
}

BOOST_FIXTURE_TEST_CASE(TestContextRetrievalFileNotExisting, ScriptingFixture) {

    BOOST_CHECK_NO_THROW(scriptingService.AddResolver(mockScriptResolver));

    const boost::filesystem::path scriptPath("SomeScriptFile.txt");

    MOCK_EXPECT(mockScriptResolver->IsFileSupported).never();
    MOCK_EXPECT(mockScriptResolver->GetContextFromFile).never();

    BOOST_CHECK_THROW(scriptingService.GetContextFromFile(scriptPath), std::invalid_argument);
}


} // namespace testing
} // namespace scripting
} // namespace core
} // namespace aw
