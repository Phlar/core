file(GLOB LuaScripting_Headers *.hpp *.h)
file(GLOB LuaScripting_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${LuaScripting_Headers})
message(STATUS "Found source files for library: " ${LuaScripting_Sources})

include_directories(.)
add_library(LibLuaScripting STATIC ${LuaScripting_Sources} ${LuaScripting_Headers})
target_link_libraries(LibLuaScripting LibBaseCore LibScriptingService LibAICore)
#set_target_properties(LibLuaScripting PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# LUA dependency
# Linker directories do not seem to be propagated properly 
#(https://cmake.org/pipermail/cmake/2011-November/047470.html)
# so we use absolute paths instead for now.
#link_directories(D:/Devel/3rdParty/lua_5.2)

include_directories(D:/Devel/3rdParty/lua_5.2/include)
target_link_libraries(LibLuaScripting "D:/Devel/3rdParty/lua_5.2/lua52.lib")

# luabind dependency
#include_directories(D:/Devel/3rdParty/luabind_0.9/include)
#link_directories(D:/Devel/3rdParty/luabind_0.9/lib/static/x64)
#if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
#    target_link_libraries(LibLuaScripting luabind09-d)
#else()
#    target_link_libraries(LibLuaScripting luabind09)
#endif()



