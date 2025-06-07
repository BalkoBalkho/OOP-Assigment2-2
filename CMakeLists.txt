cmake_minimum_required(VERSION 3.14)
project(Assigment)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG c1ab645
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()


set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/data")




# This is the main part:
set(SOURCES main.cpp raygui.h "Leaderboard.h" "Classes.hpp" "Drawing.hpp" "Units.hpp" "veryslowjson.hpp" "Units.hpp" "JsonRegistry.h" "gamestuff.h" "Drawing.cpp")
add_executable(${PROJECT_NAME} ${SOURCES} "gamestuff.h")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

# Web Configurations


# That's it! You should have an example e "raygui.h"xecutable that you can run. Have fun!