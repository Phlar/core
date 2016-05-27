file(GLOB AIService_Headers *.hpp *.h)
file(GLOB AIService_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${AIService_Headers})
message(STATUS "Found source files for library: " ${AIService_Sources})

include_directories(.)
add_library(LibAIService ${AIService_Sources} ${AIService_Headers})
target_link_libraries(LibAIService LibBase)