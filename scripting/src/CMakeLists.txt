file(GLOB ScriptingService_Headers *.hpp *.h)
file(GLOB ScriptingService_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${ScriptingService_Headers})
message(STATUS "Found source files for library: " ${ScriptingService_Sources})

include_directories(.)
add_library(LibScriptingService ${ScriptingService_Sources} ${ScriptingService_Headers})
target_link_libraries(LibBase)