cmake_minimum_required(VERSION 3.30.0-rc3)
project(Autonomous-Agents)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
    SFML 
    GIT_REPOSITORY http://github.com/SFML/SFML.git
    GIT_TAG 2.6.1)

FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/Assets/")
add_compile_definitions(ASSETS_DIR="${ASSETS_DIR}")

# Add standard build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Core module - should be visible to every class
add_subdirectory(Source)

#add_executable(Asteroids Source/Core/Source.cpp)
#target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source)
target_link_libraries(Asteroids sfml-graphics)