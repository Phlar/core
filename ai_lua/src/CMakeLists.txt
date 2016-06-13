file(GLOB AILuaExt_Headers *.hpp *.h)
file(GLOB AILuaExt_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${AILuaExt_Headers})
message(STATUS "Found source files for library: " ${AILuaExt_Sources})

include_directories(.)
add_library(LibAILuaExt STATIC ${AILuaExt_Sources} ${AILuaExt_Headers})
target_link_libraries(LibAILuaExt LibBase LibScriptingService LibAIService)
#set_target_properties(LibAILuaExt PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# LUA dependency
# Linker directories do not seem to be propagated properly 
#(https://cmake.org/pipermail/cmake/2011-November/047470.html)
# so we use absolute paths instead for now.
#link_directories(D:/Devel/3rdParty/lua_5.2)
target_include_directories(LibAILuaExt PUBLIC "D:/Devel/3rdParty/lua_5.2/include")
target_link_libraries(LibAILuaExt "D:/Devel/3rdParty/lua_5.2/lua52.lib")

# luabind dependency
target_include_directories(LibAILuaExt PUBLIC "D:/Devel/3rdParty/luabind_0.9/include")
#link_directories(D:/Devel/3rdParty/luabind_0.9/lib/static/x64)
target_link_libraries(LibAILuaExt debug "D:/Devel/3rdParty/luabind_0.9/lib/static/x64/luabind09-d.lib")
target_link_libraries(LibAILuaExt optimized "D:/Devel/3rdParty/luabind_0.9/lib/static/x64/luabind.lib")
