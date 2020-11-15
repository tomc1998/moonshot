cmake_minimum_required(VERSION 3.11)

project(game)

# Default to debug
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

########################
# Find raylib, download if requires - basically copy & pasted from raylib
# build examples
# First, try and find an installed raylib
find_package(raylib 2.5 QUIET)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    # TODO Change this, we don't just want to build master
    URL https://github.com/raysan5/raylib/archive/master.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    # Build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()
########################

########################
# Get boost 1.74.0
include(FetchContent)
FetchContent_Declare(
    boost
    URL https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.gz
)
FetchContent_GetProperties(boost)
if (NOT boost_POPULATED)
  FetchContent_Populate(Boost)
endif()
include_directories(${boost_SOURCE_DIR})
########################

# Find all source files, create executable
file(GLOB_RECURSE SRC "src/*.cpp")
add_executable(game ${SRC})
target_link_libraries(game raylib)

# Link assets directory to build directory
# TODO No ln on windows?
if(NOT EXISTS "assets")
  execute_process(COMMAND "ln" "-s" "${CMAKE_CURRENT_SOURCE_DIR}/assets" "assets")
endif()
