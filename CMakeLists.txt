cmake_minimum_required(VERSION 3.25)

# ====================================================================
# 1. AUTOMAATTINEN GEODE SDK -YMPÄRISTÖN ALUSTUS JA LATAUS VERKOSTA
# ====================================================================
include(FetchContent)

message(STATUS "[AI-MOD] Aloitetaan Geode SDK -alustustyökalujen haku verkon yli...")

FetchContent_Declare(
    GeodeFramework
    URL https://github.com
    DOWNLOAD_NO_PROGRESS FALSE
)

FetchContent_GetProperties(GeodeFramework)
if(NOT geodeframework_POPULATED)
    message(STATUS "[AI-MOD] Ladataan Geode framework pakattuna arkistona...")
    FetchContent_Populate(GeodeFramework)
    
    # Lisätään ladatun paketin polut kääntäjän etsintäreitteihin
    list(APPEND CMAKE_PREFIX_PATH "${geodeframework_SOURCE_DIR}")
    list(APPEND CMAKE_MODULE_PATH "${geodeframework_SOURCE_DIR}/cmake")
    set(Geode_DIR "${geodeframework_SOURCE_DIR}/cmake" CACHE PATH "Geode configuration directory" FORCE)
    
    message(STATUS "[AI-MOD] Geode-polut pakotettu välimuistiin: ${Geode_DIR}")
endif()

# ====================================================================
# 2. PROJEKTIN JA KÄÄNTÄJÄN ASETUKSET
# ====================================================================
project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pakotetaan Android-kohtaiset kääntäjäasetukset päälle
if(ANDROID)
    message(STATUS "[AI-MOD] Tunnistettu Android-ympäristö. Aktivoidaan optimoinnit.")
    add_compile_options(-O3 -fPIC -w)
endif()

# ====================================================================
# 3. PAKETIN ETSINTÄ JA LIITTÄMINEN
# ====================================================================
# Etsitään Geode-paketti nyt pakotetuista sisäisistä poluista
find_package(Geode REQUIRED PATHS "${Geode_DIR}" NO_DEFAULT_PATH)

if(Geode_FOUND)
    message(STATUS "[AI-MOD] GEODE SDK LÖYTYNYT ONNISTUNEESTI! Jatketaan modin kääntämiseen.")
else()
    message(FATAL_ERROR "[AI-MOD] Kriittinen virhe: Geode SDK:ta ei löytynyt pakottamisesta huolimatta.")
endif()

# ====================================================================
# 4. MODIN LOPULLINEN RAKENTAMINEN
# ====================================================================
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
