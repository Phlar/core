#include "AILuaExtensions.hpp"

#include "ServiceLocator.hpp"

#include "IScriptingService.hpp"
#include "IScriptResolver.hpp"

#include "LUAScriptResolver.hpp"
#include "LUATypeHelpers.hpp"

#include "IBlackboard.hpp"
#include "IBlackboardValue.hpp"
#include "BlackboardUtils.hpp"
#include "ITask.hpp"

#include "UUID.hpp"
#include "Utils.hpp"

#include <boost/any.hpp>
#include <boost/intrusive_ptr.hpp>

#include <luabind/luabind.hpp>

namespace aw {
namespace core {
namespace ai {
namespace lua {

namespace {

boost::any convertInt16ToTaskResult(lua_State* luaState, const aw::core::scripting::lua::FetchFromLUAFunction& getIntFromLUA) {

    try {

        const boost::any intValue = getIntFromLUA(luaState);
        const int16_t* intResult = boost::any_cast<int16_t>(&intValue);
        if(!intResult) {
            throw("Error casting to int16_t.");
        }

        // Prefer an explicit switch / case instead of static_cast because of coming addition to the enum.
        switch(*intResult) {
            case aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED: {
                return aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED;
            }
            case aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED: {
                return aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED;
            }
            case aw::core::ai::ITask::TaskResult::TASK_RESULT_RUNNING: {
                return aw::core::ai::ITask::TaskResult::TASK_RESULT_RUNNING;
            }
            default: {
                std::stringstream errorMessage;
                errorMessage << "No matching enum found in ITask::TaskResult for " << *intResult;
                throw std::runtime_error(errorMessage.str());
            }
        }
    } catch(const std::exception& e) {

        std::stringstream errorMessage;
        errorMessage << "Exception caught while fetching / converting to ITask::TaskResult:" << e.what();
        throw(std::runtime_error(errorMessage.str()));

    } catch(...) {

        throw(std::runtime_error("Unknown exception caught while fetching / converting to ITask::TaskResult."));
    }
}

} // namespace anonymous


void exposeAIInterfacesToLUA() {

    // Get the scripting service...
    scripting::IScriptingServicePtr scriptingService = boost::dynamic_pointer_cast<scripting::IScriptingService>(
        base::ServiceLocator::Instance()->GetService(scripting::ID_SCRIPTING_SERVICE));

    if(!scriptingService) {
        throw std::runtime_error("Error exposing AI interfaces to LUA: No scripting service found.");
    }

    // ...the LUA resolver...
    scripting::lua::LUAScriptResolverPtr luaScriptResolver = 
        boost::dynamic_pointer_cast<scripting::lua::LUAScriptResolver>(scriptingService->GetResolver(
        scripting::lua::ID_LUA_SCRIPT_RESOLVER));

    if(!luaScriptResolver) {
        throw std::runtime_error("Error exposing AI interfaces to LUA: No lua script-resolver found.");
    }

    std::string currentlyExposedItem;

    // Register all relevant types that should be usable from LUA scripts.
    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::def("createBlackboardValue_uint8", &aw::core::ai::support::createBlackboardValue<uint8_t>),
               luabind::def("createBlackboardValue_uint16", &aw::core::ai::support::createBlackboardValue<uint16_t>),
               luabind::def("createBlackboardValue_uint32", &aw::core::ai::support::createBlackboardValue<uint32_t>),
               luabind::def("createBlackboardValue_int8", &aw::core::ai::support::createBlackboardValue<int8_t>),
               luabind::def("createBlackboardValue_int16", &aw::core::ai::support::createBlackboardValue<int16_t>),
               luabind::def("createBlackboardValue_int32", &aw::core::ai::support::createBlackboardValue<int32_t>),
               luabind::def("createBlackboardValue_float", &aw::core::ai::support::createBlackboardValue<float>),
               luabind::def("createBlackboardValue_string", &aw::core::ai::support::createBlackboardValue<std::string>),
               luabind::def("createUUIDFromString", &aw::core::base::utils::CreateUUIDFromString);
    });

    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::ai::ITask>("ITask").enum_("constants")
            [
                luabind::value("TASK_RESULT_FAILED", aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED),
                luabind::value("TASK_RESULT_PASSED", aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED),
                luabind::value("TASK_RESULT_RUNNING", aw::core::ai::ITask::TaskResult::TASK_RESULT_RUNNING)
            ];
    });

    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::ai::IBlackboardValue, aw::core::ai::IBlackboardValuePtr>("IBlackboardValue")
            .def("GetSemanticTypeID", &aw::core::ai::IBlackboardValue::GetSemanticTypeID)
            .def("GetID", &aw::core::ai::IBlackboardValue::GetID);
    });

    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::ai::IBlackboard, aw::core::ai::IBlackboardPtr>("IBlackboard")
            .def("SetValue", &aw::core::ai::IBlackboard::SetValue)
            .def("GetValue", &aw::core::ai::IBlackboard::GetValue);
    });

    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::base::UUID>("UUID");
    });


    // Register certain "not out of the box" conversion helpers.

    // Mapping back from an int to an enum.When registering the actual
    // "pop" function another function gets injected to be
    // executed afterwards performing the actual mapping from int - enum.
    luaScriptResolver->RegisterFetchTypeFromLUAFunction(
        typeid(aw::core::ai::ITask::TaskResult),
        [](lua_State* state) {
            return convertInt16ToTaskResult(
                state,
                [](lua_State* state) {
                    return aw::core::scripting::lua::popFromLUAStack<int16_t>(state);
                }
            );
        }
    );
}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw