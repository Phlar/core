file(GLOB LuaScripting_Headers *.hpp *.h)
file(GLOB LuaScripting_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${LuaScripting_Headers})
message(STATUS "Found source files for library: " ${LuaScripting_Sources})

include_directories(.)
add_library(LibLuaScripting ${LuaScripting_Sources} ${LuaScripting_Headers})
target_link_libraries(LibLuaScripting LibBaseCore LibScriptingService LibAICore)
set_target_properties(LibLuaScripting PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# LUA dependency
include_directories(D:/Devel/3rdParty/lua_5.2/include)
link_directories(D:/Devel/3rdParty/lua_5.2)
target_link_libraries(LibLuaScripting lua52)

# luabind dependency
include_directories(D:/Devel/3rdParty/luabind_0.9/include)
link_directories(D:/Devel/3rdParty/luabind_0.9/lib/static/x64)
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    target_link_libraries(LibLuaScripting luabind09-d)
else()
    target_link_libraries(LibLuaScripting luabind09)
endif()



