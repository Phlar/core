file(GLOB AICore_Headers src/*.hpp src/*.h)
file(GLOB AICore_Sources src/*.cpp src/*.c)

message(STATUS "Found headers for library:" ${AICore_Headers})
message(STATUS "Found source files for library: " ${AICore_Sources})

include_directories(src)
add_library(LibAICore ${AICore_Sources} ${AICore_Headers})
