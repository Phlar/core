#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_LUABasics"

#include "LUAScriptResolver.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <vector>

namespace aw {
namespace core {
namespace scripting {
namespace lua {
namespace testing {


struct LUATestFixture {

    LUATestFixture()
    : luaResolver()
    , luaTestFilePath("D:/Devel/core/extensions/lua/testing/testdata/TestFunctions.lua") {
    }

    IScriptContextPtr getCheckedContext(const boost::filesystem::path& scriptPath) {

        IScriptContextPtr ctx;
        BOOST_REQUIRE_NO_THROW(ctx = luaResolver.GetContext(scriptPath));
        BOOST_REQUIRE(ctx);
        return ctx;
    }

    LUAScriptResolver luaResolver;
    boost::filesystem::path luaTestFilePath;
};


BOOST_FIXTURE_TEST_CASE(TestLUASimpleFunctionWithoutParameters, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector noArgs;
    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncNoParam", noArgs));
}

BOOST_FIXTURE_TEST_CASE(TestLUAUnknownFunctionShouldThrow, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector noArgs;
    BOOST_CHECK_THROW(ctx->ExecuteScript("GarbageFunction", noArgs), std::runtime_error);
}

} // namespace testing
} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw