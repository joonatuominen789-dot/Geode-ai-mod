cmake_minimum_required(VERSION 3.25)

project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

# Sisällytetään virallinen Geode SDK kääntöjärjestelmä mukaan.
# GitHub Actions -työkalu tarjoaa tämän polun automaattisesti.
include($ENV{GEODE_CLI}/cmake/Geode.cmake)

# Luodaan virallinen Geode-modi Androidille
add_geode_mod(
    ${PROJECT_NAME}
    VERSION 1.0.0
    SOURCES src/main.cpp
)
