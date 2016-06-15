#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AiLUAExtBasics"

#include "AILuaExtensions.hpp"

#include "ServiceLocator.hpp"
#include "LUAScriptResolver.hpp"
#include "InitScriptingService.hpp"

#include "IBehaviorTree.hpp"
#include "IBlackboard.hpp"
#include "IScriptAction.hpp"
#include "IScriptingService.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace aw {
namespace core {
namespace ai {
namespace lua {
namespace testing {


struct TestFixture {

    TestFixture() {

        BOOST_REQUIRE_NO_THROW(serviceLocator = base::ServiceLocator::Instance());
    }

    base::ServiceLocatorPtr serviceLocator;
};


BOOST_FIXTURE_TEST_CASE(TestNoScriptingServiceShouldThrow, TestFixture) {

    BOOST_CHECK_THROW(exposeAIInterfacesToLUA(), std::exception);
}

BOOST_FIXTURE_TEST_CASE(TestNoScriptResolverShouldThrow, TestFixture) {

    scripting::IScriptingServicePtr scriptingService = scripting::RegisterService(serviceLocator);
    BOOST_CHECK_THROW(exposeAIInterfacesToLUA(), std::exception);
}

BOOST_FIXTURE_TEST_CASE(TestSample, TestFixture) {

    scripting::IScriptingServicePtr scriptingService;
    BOOST_REQUIRE_NO_THROW(scriptingService = scripting::RegisterService(serviceLocator));
    BOOST_REQUIRE(scriptingService);

    scripting::lua::LUAScriptResolverPtr luaResolver;
    BOOST_REQUIRE_NO_THROW(luaResolver = scripting::lua::LUAScriptResolverPtr(new scripting::lua::LUAScriptResolver()));
    BOOST_REQUIRE(luaResolver);
    BOOST_REQUIRE_NO_THROW(scriptingService->AddResolver(luaResolver));

    BOOST_CHECK_NO_THROW(exposeAIInterfacesToLUA());

    // Todo: Refactor Scripting-Action / factory to create an action based on a string.
    //       Set up a simple behavior-tree havin one action attached - run tests...



}



} // namespace testing
} // namespace lua
} // namespace ai
} // namespace core
} // namespace aw
