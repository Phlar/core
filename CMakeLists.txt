project(Core)
cmake_minimum_required(VERSION 3.1.2 FATAL_ERROR)


#"Treat warning as errors"
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()


# Directory specifying where to look up the 3rdParty dependencies
set(3RD_PARTY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/ CACHE PATH "Root directory of all 3rdParty dependencies" )

# Try resolving the BOOST libraries
set(BOOST_ROOT D:/Devel/3rdParty/boost_1_57_0/)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
message(STATUS "BoostDir: ${BOOST_ROOT}")
#find_package(Boost 1.57 REQUIRED)
find_package(Boost 1.57 COMPONENTS system date_time chrono serialization filesystem program_options random thread regex unit_test_framework)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

message(STATUS "Boost Version:" ${Boost_VERSION})
message(STATUS "Boost Include Dirs:" ${Boost_INCLUDE_DIR})
message(STATUS "Boost Libraries:")
foreach(lib ${Boost_LIBRARIES})
        message(STATUS ${lib})
endforeach()

enable_testing()

add_subdirectory(./ai)