file(GLOB LoggingService_Headers *.hpp *.h)
file(GLOB LoggingService_Sources *.cpp *.c)

message(STATUS "Found headers for library:" ${LoggingService_Headers})
message(STATUS "Found source files for library: " ${LoggingService_Sources})

include_directories(.)
add_library(LibLoggingService ${LoggingService_Sources} ${LoggingService_Headers})
target_link_libraries(LibLoggingService)