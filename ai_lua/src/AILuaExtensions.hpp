#pragma once

namespace aw {
namespace core {
namespace ai {
namespace lua {

//! \brief Bridging the gap between the C++ AI interfaces and LUA,
//! meaning after calling this function one can use the exposed 
//! interfaces within LUA code. This function requires having a
//! valid LUA script resolver registered at the ScriptingService.
void exposeAIInterfacesToLUA();

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
