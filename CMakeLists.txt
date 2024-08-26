cmake_minimum_required(VERSION 3.11)
project(rayPong)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(${PROJECT_NAME}
    src/main.c
)

# Set paths to local Raylib
set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/raylib")
set(RAYLIB_INCLUDE_DIR "${RAYLIB_DIR}/include")
set(RAYLIB_LIBRARY "${RAYLIB_DIR}/lib/libraylib.a")

# Check if the local Raylib library exists
if(EXISTS ${RAYLIB_LIBRARY})
    message(STATUS "Using local Raylib from ${RAYLIB_DIR}")

    # Include the Raylib headers
    target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${RAYLIB_INCLUDE_DIR}
    )

    # Link the Raylib library manually
    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${RAYLIB_LIBRARY}
        m
    )
else()
    message(STATUS "Local Raylib not found, downloading...")

    # Dependencies
    set(RAYLIB_VERSION 5.0)

    # Fetch and build Raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded Raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()

    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
        m
    )
endif()

# Compiler options
target_compile_options(${PROJECT_NAME}
PRIVATE
    -Werror
    -Wall
    -Wpedantic
    -Wconversion
    -Wextra
)
target_compile_options(${PROJECT_NAME}
PRIVATE
    -O3
    -mtune=native
)
