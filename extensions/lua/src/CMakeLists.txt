file(GLOB LuaScripting_Headers *.hpp *.h)
file(GLOB LuaScripting_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${LuaScripting_Headers})
message(STATUS "Found source files for library: " ${LuaScripting_Sources})

include_directories(.)
add_library(LibLuaScripting ${LuaScripting_Sources} ${LuaScripting_Headers})
target_link_libraries(LibLuaScripting LibBaseCore LibScripingService LibAICore)
set_target_properties(LibLuaScripting PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
