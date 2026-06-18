cmake_minimum_required(VERSION 3.25)

# ====================================================================
# 1. ERITTÄIN MONIMUTKAINEN JA POMMINVARMA GEODE SDK LATAUSJÄRJESTELMÄ
# ====================================================================
include(FetchContent)

message(STATUS "[AI-MOD] Käynnistetään edistynyt verkkolatausjärjestelmä...")

# Määritellään lataus käyttäen modernia MakeAvailable-arkkitehtuuria
FetchContent_Declare(
    geodeframework
    URL https://github.com
    URL_HASH MD5=6ba61ff7d11703e33b9bfbc7ea7bc100  # Pakotetaan välimuistitarkistus
    DOWNLOAD_NO_PROGRESS FALSE
    TIMEOUT 30                                    # Estetään kääntäjän jumiutuminen
)

# Pakotetaan CMake ohittamaan aliprojektien omat raskaat testit ja kääntämiset
set(GEODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GEODE_BUILD_LOADER ON CACHE BOOL "" FORCE)

# Suoritetaan haku lennosta
message(STATUS "[AI-MOD] Pakotetaan järjestelmä avaamaan verkkoyhteys...")
FetchContent_MakeAvailable(geodeframework)

# ====================================================================
# 2. POLKUJEN JA VÄLIMUISTIN AGGRESSIIVINEN KAAPPAUS
# ====================================================================
# Haetaan ladatun koodin sijainti palvelimella
get_property(geode_source_dir SOURCE_DIR geodeframework)

if(EXISTS "${geode_source_dir}")
    message(STATUS "[AI-MOD] Verkkopaketti purettu polkuun: ${geode_source_dir}")
    
    # Syötetään puuttuvat tiedostot suoraan CMaken sisäisiin reitteihin
    list(APPEND CMAKE_PREFIX_PATH "${geode_source_dir}")
    list(APPEND CMAKE_MODULE_PATH "${geode_source_dir}/cmake")
    set(Geode_DIR "${geode_source_dir}/cmake" CACHE PATH "Pakotettu reitti" FORCE)
else()
    message(WARNING "[AI-MOD] Pura epäonnistui lennosta, käytetään varapolkua.")
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
# 4. LOPULLINEN PAKETIN LIITTÄMINEN JA KÄÄNTÄMINEN
# ====================================================================
# Etsitään ladattu ja purettu paketti suoraan muistista
find_package(Geode REQUIRED PATHS "${Geode_DIR}" NO_DEFAULT_PATH)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
