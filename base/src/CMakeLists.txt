file(GLOB Base_Headers *.hpp *.h)
file(GLOB Base_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${Base_Headers})
message(STATUS "Found source files for library: " ${Base_Sources})

include_directories(.)
add_library(LibBase ${Base_Sources} ${Base_Headers})

set_target_properties(LibBase PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
