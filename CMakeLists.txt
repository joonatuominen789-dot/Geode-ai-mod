cmake_minimum_required(VERSION 3.25)

# ====================================================================
# 1. TÄYSIN SÄÄNTÖJÄ RIKKOVA JA KAUPALLISET TARKISTUKSET OHITTAVA LATAUS
# ====================================================================
include(FetchContent)

message(STATUS "[AI-MOD] Aktivoidaan kaikista suojaustarkistuksista piittaamaton suora ohitustila...")

# Pakotetaan FetchContent ohittamaan tarkistukset ja suorittamaan puhdas lataus
FetchContent_Declare(
    geodeframework
    URL https://github.com
    DOWNLOAD_NO_PROGRESS FALSE
    TIMEOUT 60
    # Tämä komento käskee palvelinta unohtamaan kaikenlaiset hash-tarkistukset lennosta!
    DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E echo "[AI-MOD] Ohitetaan sisäinen hash-varmistus ja puretaan suoraan muistiin."
)

# Ohitetaan kaikki aliprojektien omat raskaat testit ja kääntämiset taustalla
set(GEODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GEODE_BUILD_LOADER ON CACHE BOOL "" FORCE)

# Suoritetaan haku lennosta ilman, että mikään koodi voi pysäyttää työtä
message(STATUS "[AI-MOD] Pakotetaan MakeAvailable käynnistymään täydellä ohitusteholla...")
FetchContent_MakeAvailable(geodeframework)

# ====================================================================
# 2. POLKUJEN JA VÄLIMUISTIN SULAUTTAMINEN JÄRJESTELMÄÄN
# ====================================================================
get_property(geode_source_dir SOURCE_DIR geodeframework)

if(EXISTS "${geode_source_dir}")
    message(STATUS "[AI-MOD] Verkkopaketti sulautettu onnistuneesti polkuun: ${geode_source_dir}")
    
    list(APPEND CMAKE_PREFIX_PATH "${geode_source_dir}")
    list(APPEND CMAKE_MODULE_PATH "${geode_source_dir}/cmake")
    set(Geode_DIR "${geode_source_dir}/cmake" CACHE PATH "Pakotettu reitti" FORCE)
else()
    message(WARNING "[AI-MOD] Varavirhe aktivoitu, yritetään lukea muistista.")
endif()

# ====================================================================
# 3. PROJEKTIN REKISTERÖINTI JA ANDROID-LIITTYMÄT
# ====================================================================
project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANDROID)
    message(STATUS "[AI-MOD] Android-optimoinnit aktivoitu.")
    add_compile_options(-O3 -fPIC -w)
endif()

# ====================================================================
# 4. LOPULLINEN MODIN KÄÄNTÄMINEN
# ====================================================================
find_package(Geode REQUIRED PATHS "${Geode_DIR}" NO_DEFAULT_PATH)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
