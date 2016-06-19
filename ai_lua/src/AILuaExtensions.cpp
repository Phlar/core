#include "AILuaExtensions.hpp"

#include "ServiceLocator.hpp"

#include "IScriptingService.hpp"
#include "IScriptResolver.hpp"

#include "LUAScriptResolver.hpp"

#include "IBlackboard.hpp"
#include "IBlackboardValue.hpp"
#include "BlackboardUtils.hpp"
#include "ITask.hpp"

#include "UUID.hpp"
#include "Utils.hpp"

#include <boost/intrusive_ptr.hpp>

#include <luabind/luabind.hpp>

namespace aw {
namespace core {
namespace ai {
namespace lua {


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

    // ...register functors exposing all relevant interfaces:
    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::def("createBlackBoardValue_uint8", &aw::core::ai::support::createBlackBoardValue<uint8_t>),
               luabind::def("createBlackBoardValue_uint16", &aw::core::ai::support::createBlackBoardValue<uint16_t>),
               luabind::def("createBlackBoardValue_uint32", &aw::core::ai::support::createBlackBoardValue<uint32_t>),
               luabind::def("createBlackBoardValue_int8", &aw::core::ai::support::createBlackBoardValue<int8_t>),
               luabind::def("createBlackBoardValue_int16", &aw::core::ai::support::createBlackBoardValue<int16_t>),
               luabind::def("createBlackBoardValue_int32", &aw::core::ai::support::createBlackBoardValue<int32_t>),
               luabind::def("createBlackBoardValue_string", &aw::core::ai::support::createBlackBoardValue<std::string>);
    });

    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::ai::ITask>("ITask").enum_("constants")
            [
                luabind::value("TASK_RESULT_FAILED", aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED),
                luabind::value("TASK_RESULT_PASSED", aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED),
                luabind::value("TASK_RESULT_RUNNING", aw::core::ai::ITask::TaskResult::TASK_RESULT_RUNNING)
            ];
    });




    // IBlackboardValuePtr
    // base::UUID
    // AIService
    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::ai::IBlackboardValue, aw::core::ai::IBlackboardValuePtr>("IBlackboardValuePtr")
            .def("GetTypeID", &aw::core::ai::IBlackboardValue::GetTypeID)
            .def("GetID", &aw::core::ai::IBlackboardValue::GetID);
    });

    luaScriptResolver->AddTypeRegistrationFunction([]() -> luabind::scope {
        return luabind::class_<aw::core::ai::IBlackboard, aw::core::ai::IBlackboardPtr>("IBlackboardPtr")
            .def("SetValue", &aw::core::ai::IBlackboard::SetValue)
            .def("GetValue", &aw::core::ai::IBlackboard::GetValue);
    });



    /*
    auto registerClass = []() -> luabind::scope {
        return luabind::class_<aw::core::ai::IBlackboard, aw::core::ai::IBlackboardPtr>("IMemberFunctionHelperTestClass")
            .def("MemberFunction", &IMemberFunctionHelperTestClass::MemberFunction)
            .def("GetReferenceCount", &IMemberFunctionHelperTestClass::GetReferenceCount)
            .def("MemberFunctionThrows", &IMemberFunctionHelperTestClass::MemberFunctionThrows);
    };
    */
    /*
    auto regFnc = []() -> luabind::scope {

        return luabind::class_<aw::core::ai::ITask>("ITask").enum_("TaskResult")
                [
                    luabind::value("TASK_RESULT_FAILED", aw::core::ai::ITask::TaskResult::TASK_RESULT_FAILED),
                    luabind::value("TASK_RESULT_PASSED", aw::core::ai::ITask::TaskResult::TASK_RESULT_PASSED),
                    luabind::value("TASK_RESULT_RUNNING", aw::core::ai::ITask::TaskResult::TASK_RESULT_RUNNING)
                ];
    };
    */








    //! \brief Returns all values matching by type-ID.
    // virtual IBlackboardValuePtr GetValue(const base::UUID& semanticTypeID) const = 0;








}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw