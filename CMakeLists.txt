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

#Nlohmann JSON
set(NLOHMANN_DIR "C:/C++ Libraries/JSON")

#CONFIG makes SFML::Window and such possible
find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)

#nlohmann
include_directories(${NLOHMANN_DIR})

# .exe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
# Application icon
set(APP_ICON_RC "${CMAKE_CURRENT_BINARY_DIR}/icon.rc")
file(WRITE ${APP_ICON_RC} "IDI_ICON1 ICON \"${CMAKE_CURRENT_SOURCE_DIR}/assets/ico/favicon.ico\"\n")
add_executable(${PROJECT_NAME} ${SOURCES} ${APP_ICON_RC})

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE src)

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