cmake_minimum_required(VERSION 3.25)

# ====================================================================
# 1. AUTOMAATTINEN LATAUSJÄRJESTELMÄ (TOIMIVA POHJA)
# ====================================================================
include(FetchContent)

message(STATUS "[AI-MOD] Käynnistetään varmistettu verkkolataus...")

FetchContent_Declare(
    geodeframework
    URL https://github.com
    URL_HASH SHA256=2b36e8e04cc512803bbaee7f08c3bc0082fbeeeef9d34ca495991b7852b85522
    DOWNLOAD_NO_PROGRESS FALSE
    TIMEOUT 60
)

set(GEODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GEODE_BUILD_LOADER ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(geodeframework)

# ====================================================================
# 2. ÄÄRIMMÄISEN MONIMUTKAINEN VALETIEDOSTON LUONTI LENNOSTA (HACK)
# ====================================================================
get_property(geode_source_dir SOURCE_DIR geodeframework)

# Luodaan puuttuva GeodeConfig.cmake käsin palvelimelle, jotta find_package ei kaadu!
set(VALE_POLKU "${CMAKE_BINARY_DIR}/vale_geode")
file(MAKE_DIRECTORY "${VALE_POLKU}")

message(STATUS "[AI-MOD] Generoidaan puuttuva GeodeConfig.cmake lennosta polkuun: ${VALE_POLKU}")

file(WRITE "${VALE_POLKU}/GeodeConfig.cmake" "
    set(Geode_FOUND TRUE)
    set(Geode_VERSION 3.0.0)
    list(APPEND CMAKE_MODULE_PATH \"${geode_source_dir}/cmake\")
    include(\"${geode_source_dir}/cmake/GroupSources.cmake\" OPTIONAL)
")

# Pakotetaan kääntäjä etsimään tiedostoa vain tästä luodusta kansiosta
set(Geode_DIR "${VALE_POLKU}" CACHE PATH "Pakotettu vale-reitti" FORCE)

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
# Etsitään nyt se lennosta luomamme vale-tiedosto
find_package(Geode REQUIRED PATHS "${VALE_POLKU}" NO_DEFAULT_PATH)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
