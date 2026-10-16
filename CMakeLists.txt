cmake_minimum_required(VERSION 3.10)
project(ottu-target C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
include(CTest)

# ============================================================================
# Build Options
# ============================================================================

option(OTTU_BUILD_EXAMPLES "Build example applications" ON)

# ============================================================================
# Unity Framework (via subdirectory)
# ============================================================================

# Enable Unity extensions
set(UNITY_EXTENSION_FIXTURE ON CACHE BOOL "" FORCE)
set(UNITY_EXTENSION_MEMORY ON CACHE BOOL "" FORCE)

add_subdirectory(libs/unity)

# ============================================================================
# Ottu Target Library
# ============================================================================

add_library(ottunity
    src/ottunity/ottunity_priv.c
)

target_include_directories(ottunity
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ottunity
        ${CMAKE_CURRENT_SOURCE_DIR}/config
)

target_link_libraries(ottunity
    PUBLIC
        unity::framework
)

# ============================================================================
# Examples (Optional)
# ============================================================================

if(OTTU_BUILD_EXAMPLES)
    add_subdirectory(examples/ottunity)
endif()
