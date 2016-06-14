#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AiLUAExtBasics"

#include "AILuaExtensions.hpp"

#include "ServiceLocator.hpp"
#include "LUAScriptResolver.hpp"
#include "InitScriptingService.hpp"

#include "IBehaviorTree.hpp"
#include "IBlackboard.hpp"
#include "IScriptAction.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace aw {
namespace core {
namespace ai {
namespace lua {


struct TestFixture {

    TestFixture() {

        BOOST_REQUIRE_NO_THROW(serviceLocator = base::ServiceLocator::Instance());
    }

    base::ServiceLocatorPtr serviceLocator;
};


BOOST_FIXTURE_TEST_CASE(TestNoScriptingServiceShouldThrow, TestFixture) {

    BOOST_CHECK_THROW(exposeAIInterfacesToLUA(), std::exception);
}

BOOST_FIXTURE_TEST_CASE(TestNoSCrriptResolverShouldThrow, TestFixture) {

    scripting::RegisterService(serviceLocator);
    BOOST_CHECK_THROW(exposeAIInterfacesToLUA(), std::exception);
}




} // namespace lua
} // namespace ai
} // namespace core
} // namespace aw
