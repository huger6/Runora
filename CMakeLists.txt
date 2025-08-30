cmake_minimum_required(VERSION 3.20)
project(Runora LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_STATIC_LIBRARIES TRUE)
if(SFML_STATIC_LIBRARIES)
    add_definitions(-DSFML_STATIC)
endif()

set(SFML_ROOT "C:/C++ Libraries/SFML-3.0.0")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

#CONFIG makes SFML::Window and such possible
find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

# .exe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link exported targets 
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System)

# Assets
set(ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_PATH="${ASSETS_PATH}")

# Warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()