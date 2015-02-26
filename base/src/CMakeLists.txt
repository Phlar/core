file(GLOB BaseCore_Headers *.hpp *.h)
file(GLOB BaseCore_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${BaseCore_Headers})
message(STATUS "Found source files for library: " ${BaseCore_Sources})

include_directories(.)
add_library(LibBaseCore ${BaseCore_Sources} ${BaseCore_Headers})

set_target_properties(LibBaseCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
