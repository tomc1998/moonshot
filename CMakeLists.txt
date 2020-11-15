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
    # For some reason, MTL support errors when building on my laptop
    set(SUPPORT_FILEFORMAT_MTL OFF CACHE BOOL "" FORCE) 
    # Build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()
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

## Set warning levels
#if(MSVC)
#  target_compile_options(game PRIVATE /W4 /WX)
#  # TODO disable dumb warnings, see gcc/clang equivalents in other if branch
#else()
#  target_compile_options(game PRIVATE -Wall -Wextra -Werror
#    # Dumb warnings I don't care about:
#    ## 'reorder' warns against initialising class fields in a different order
#    ## to their declaration - i don't find this particularly useful & it makes
#    ## refactoring a huge pain
#    -Wno-reorder
#    ## 'unused-parameter' warns when parameters aren't used - but it's useful
#    ## to have unused parameters sometimes, e.g. when future-proofing in the
#    ## short term
#    -Wno-unused-parameter
#    ## Declaring loop variables as `int` is useful, for cases where you write
#    ## the expression `i-1` for whatever reason, and don't want to end up with
#    ## an int overflow bug. 'sign-compare' forces you to cast the result of
#    ## vector::size() every single time you use it as a loop boundary, which is a
#    ## pain & not worth it imo
#    -Wno-sign-compare
#
#    # Unfortunately raymath.h is a C11 header, so it uses the 0 struct
#    # initializer - this shouldn't really cause any bugs, but it does pop up a
#    # bunch of C++ warnings. For now, just disable the missing initializer
#    # warning...
#    # TODO use c++ raymath alternative?
#    -Wno-missing-field-initializers
#    )
#endif()

