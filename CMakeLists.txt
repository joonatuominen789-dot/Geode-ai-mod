cmake_minimum_required(VERSION 3.25)

# Haetaan Geoden virallinen kääntäjäapu verkon yli automaattisesti
if(NOT GEODE_CLI)
    include(FetchContent)
    FetchContent_Declare(
        GeodeFetch
        GIT_REPOSITORY https://github.com
        GIT_TAG v3.0.0
    )
    FetchContent_MakeAvailable(GeodeFetch)
endif()

project(AIEditorMod)

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
