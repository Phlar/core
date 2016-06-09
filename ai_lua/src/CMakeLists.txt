file(GLOB AILuaExt_Headers *.hpp *.h)
file(GLOB AILuaExt_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${AILuaExt_Headers})
message(STATUS "Found source files for library: " ${AILuaExt_Sources})

include_directories(.)
add_library(LibAILuaExt ${AILuaExt_Sources} ${AILuaExt_Headers})
target_link_libraries(LibAILuaExt LibBase LibScriptingService)