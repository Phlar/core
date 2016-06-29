#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AiLUAExtBasics"

#include "AILuaExtensions.hpp"

#include "ServiceLocator.hpp"
#include "LUAScriptResolver.hpp"
#include "InitScriptingService.hpp"

#include "LUATypeHelpers.hpp"

#include "IAIService.hpp"
#include "IBehaviorTree.hpp"
#include "IBlackboard.hpp"
#include "IScriptAction.hpp"
#include "IScriptingService.hpp"
#include "InitAIService.hpp"
#include "BlackboardUtils.hpp"

#include <boost/any.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>


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

        luaResolver->RegisterPushTypeToLUAFunction(typeid(aw::core::ai::IBlackboardPtr), scripting::lua::pushToLUAStack<aw::core::ai::IBlackboardPtr>);
    }

    base::ServiceLocatorPtr serviceLocator;

    IAIServicePtr aiService;
    scripting::IScriptingServicePtr scriptingService;
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

BOOST_FIXTURE_TEST_CASE(TestResturnTaskResultFromLUAScript, TestFixture) {

    BOOST_CHECK_NO_THROW(exposeAIInterfacesToLUA());

    const std::string scriptSource = "function testFunction(blackboard)"
                                     "    print(\"Returning from LUA-script...\" .. ITask.TASK_RESULT_PASSED)"
                                     "    return (ITask.TASK_RESULT_PASSED)"
                                     "end";

    IBlackboardPtr blackboard = aiService->createBlackboard();

    IScriptActionPtr scriptAction;
    BOOST_CHECK_NO_THROW(scriptAction = aiService->createScriptActionFromString(
                         scriptSource, scripting::lua::ID_LUA_SCRIPT_RESOLVER, "testFunction", false));
    BOOST_REQUIRE(scriptAction);

    aw::core::ai::ITask::TaskResult result = aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED;
    BOOST_CHECK_NO_THROW(result = scriptAction->Evaluate(blackboard, nullptr));
    BOOST_CHECK(result == aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED);
}

BOOST_FIXTURE_TEST_CASE(TestCreateBlackboardParameterInLUAScript, TestFixture) {

    BOOST_CHECK_NO_THROW(exposeAIInterfacesToLUA());

    const std::string scriptSource = "function testFunction(blackboard)"
                                     "    myUUID = createUUIDFromString(\"{11111111-2222-3333-4444-555555555555}\")"
                                     "    myValue = createBlackBoardValue_string(myUUID, \"Value created in LUA.\")"
                                     "    blackboard:SetValue(myValue)"
                                     "    return (ITask.TASK_RESULT_PASSED)"
                                     "end";

    IBlackboardPtr blackboard = aiService->createBlackboard();

    IScriptActionPtr scriptAction;
    BOOST_CHECK_NO_THROW(scriptAction = aiService->createScriptActionFromString(
                         scriptSource, scripting::lua::ID_LUA_SCRIPT_RESOLVER, "testFunction", false));
    BOOST_REQUIRE(scriptAction);

    aw::core::ai::ITask::TaskResult result = aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED;
    BOOST_CHECK_NO_THROW(result = scriptAction->Evaluate(blackboard, nullptr));
    BOOST_CHECK(result == aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED);

    std::string blackboardString;
    IBlackboardValuePtr blackboardValue;
    BOOST_CHECK_NO_THROW(blackboardString = aw::core::ai::support::getRawValueFromBlackboard<std::string>(
        blackboard,
        aw::core::base::utils::CreateUUIDFromString("{11111111-2222-3333-4444-555555555555}")));
    BOOST_CHECK_EQUAL(blackboardString, "Value created in LUA.");
}


} // namespace testing
} // namespace lua
} // namespace ai
} // namespace core
} // namespace aw
