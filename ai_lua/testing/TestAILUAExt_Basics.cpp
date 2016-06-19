#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AiLUAExtBasics"

#include "AILuaExtensions.hpp"

#include "ServiceLocator.hpp"
#include "LUAScriptResolver.hpp"
#include "InitScriptingService.hpp"

#include "IAIService.hpp"
#include "IBehaviorTree.hpp"
#include "IBlackboard.hpp"
#include "IScriptAction.hpp"
#include "IScriptingService.hpp"
#include "InitAIService.hpp"

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
        BOOST_REQUIRE(serviceLocator);

        // Register services...
        BOOST_REQUIRE_NO_THROW(aiService = ai::RegisterService(serviceLocator));
        BOOST_REQUIRE(aiService);

        BOOST_REQUIRE_NO_THROW(scriptingService = scripting::RegisterService(serviceLocator));
        BOOST_REQUIRE(scriptingService);

        // Register LUA resolver...
        scripting::lua::LUAScriptResolverPtr luaResolver;
        BOOST_REQUIRE_NO_THROW(luaResolver = scripting::lua::LUAScriptResolverPtr(new scripting::lua::LUAScriptResolver()));
        BOOST_REQUIRE(luaResolver);
        BOOST_REQUIRE_NO_THROW(scriptingService->AddResolver(luaResolver));

        // Set up a tree having one action only.
        BOOST_REQUIRE_NO_THROW(behaviorTree = aiService->createBehaviorTree());
        BOOST_REQUIRE(behaviorTree);
        BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptAction());
        BOOST_REQUIRE(scriptAction);
        BOOST_REQUIRE_NO_THROW(behaviorTree->SetRoot(scriptAction));
    }

    base::ServiceLocatorPtr serviceLocator;

    IAIServicePtr aiService;
    scripting::IScriptingServicePtr scriptingService;

    IBehaviorTreePtr behaviorTree;
    IScriptActionPtr scriptAction;
};

BOOST_AUTO_TEST_CASE(TestNoScriptingServiceShouldThrow) {

    base::ServiceLocatorPtr serviceLocator;
    BOOST_REQUIRE_NO_THROW(serviceLocator = base::ServiceLocator::Instance());
    BOOST_REQUIRE(serviceLocator);
    BOOST_CHECK_THROW(exposeAIInterfacesToLUA(), std::exception);
}

BOOST_AUTO_TEST_CASE(TestNoScriptResolverShouldThrow) {

    base::ServiceLocatorPtr serviceLocator;
    BOOST_REQUIRE_NO_THROW(serviceLocator = base::ServiceLocator::Instance());
    BOOST_REQUIRE(serviceLocator);

    scripting::IScriptingServicePtr scriptingService = scripting::RegisterService(serviceLocator);
    BOOST_CHECK_THROW(exposeAIInterfacesToLUA(), std::exception);
}

BOOST_FIXTURE_TEST_CASE(TestSample, TestFixture) {

    BOOST_CHECK_NO_THROW(exposeAIInterfacesToLUA());

    const std::string scriptSource = "function FirstTest()"
                                     "end";

    BOOST_CHECK_NO_THROW(scriptAction->SetScriptString(scriptSource, scripting::lua::ID_LUA_SCRIPT_RESOLVER, "FirstTest", false));

    BehaviorTreeState executionState = BehaviorTreeState::STATE_NOT_RUN;
    BOOST_CHECK_NO_THROW(executionState = behaviorTree->ExecuteSync());
    BOOST_CHECK(executionState == BehaviorTreeState::STATE_FAILED);
}



} // namespace testing
} // namespace lua
} // namespace ai
} // namespace core
} // namespace aw
