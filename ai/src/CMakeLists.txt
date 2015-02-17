file(GLOB AICore_Headers *.hpp *.h)
file(GLOB AICore_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${AICore_Headers})
message(STATUS "Found source files for library: " ${AICore_Sources})

include_directories(.)
add_library(LibAICore ${AICore_Sources} ${AICore_Headers})