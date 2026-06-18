cmake_minimum_required(VERSION 3.25)

project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

# Kerrotaan CMakelle mistä Geode SDK:n asetukset löytyvät GitHub Actionsissa
list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK}")

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    VERSION 1.0.0
    SOURCES src/main.cpp
)
