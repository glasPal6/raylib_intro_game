cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib_intro_game)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)

# Our Project
add_executable(${PROJECT_NAME} 
    src/main.c
)
target_link_libraries(${PROJECT_NAME} 
    raylib
)
