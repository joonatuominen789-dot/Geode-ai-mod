cmake_minimum_required(VERSION 3.25)

# 1. Haetaan Geode CLI:n ja kääntäjän asetukset (Tämä puuttui valepohjasta)
if(NOT DEFINED GEODE_CLI)
    message(FATAL_ERROR "Geode CLI:tä ei löytynyt! Varmista, että käytät Geode GitHub Actionsia.")
endif()

project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

# 2. Sisällytetään virallinen Geode SDK kääntöjärjestelmä mukaan
include(${GEODE_CLI}/cmake/Geode.cmake)

# 3. Luodaan virallinen Geode-modi (Tämä luo sen puuttuvan .geode -tiedoston)
add_geode_mod(
    ${PROJECT_NAME}
    VERSION 1.0.0
    SOURCES src/main.cpp
)
