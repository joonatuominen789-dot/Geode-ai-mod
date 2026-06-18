cmake_minimum_required(VERSION 3.25)
project(AIEditorMod)

# Ladattavan Geode SDK:n tarkka versio
set(GEODE_VERSION 4.0.0)

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
