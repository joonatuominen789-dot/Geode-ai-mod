cmake_minimum_required(VERSION 3.25)

project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

# Tämä komento etsii ja lataa Geode SDK:n automaattisesti GitHub Actionsista
find_package(Geode REQUIRED)

# Luodaan virallinen Geode-modi Androidille
add_geode_mod(
    ${PROJECT_NAME}
    VERSION 1.0.0
    SOURCES src/main.cpp
)
