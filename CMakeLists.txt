project(Core)

#"Treat warning as errors"
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()


# Directory specifying where to look up the 3rdParty dependencies
set( 3RD_PARTY_DIRECTORY ../3rdParty CACHE PATH "Root directory of all 3rdParty dependencies" )

# Try resolving the BOOST libraries
set(BOOST_ROOT ${3RD_PARTY_DIRECTORY}/boost_1_57_0)
message(STATUS "BoostDir: ${BOOST_ROOT}")
find_package(Boost 1.57 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

cmake_minimum_required(VERSION 3.1.2 FATAL_ERROR)

add_subdirectory(./ai)