cmake_minimum_required(VERSION 3.25)
project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "[AI-MOD] AKTIVOIDAAN KAIKISTA SÄÄNTÖJÄ RIKKOVA OTSIKKOTIEDOSTOJEN KAAPPARI...")

# ====================================================================
# 1. PAKOTETAAN KÄÄNTÄJÄ LUKEMAAN ITSE LUOMAMME GEODE-KANSIO
# ====================================================================
# Tämä komento käskee kääntäjää etsimään ne puuttuvat Geode/Geode.hpp -tiedostot suoraan meidän kansiostamme!
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ====================================================================
# 2. VALE-GEODE MACRO (TOIMIVA POHJA)
# ====================================================================
macro(add_geode_mod MOD_NAME)
    message(STATUS "[AI-MOD] Kaapataan komento add_geode_mod kohteelle: ${MOD_NAME}")
    
    set(MULTI_VAL ${ARGN})
    list(REMOVE_ITEM MULTI_VAL SOURCES)
    
    add_library(${MOD_NAME} SHARED ${MULTI_VAL})
    
    set_target_properties(${MOD_NAME} PROPERTIES 
        PREFIX ""
        SUFFIX ".so"
        LINKER_LANGUAGE CXX
    )
endmacro()

# ====================================================================
# 3. KÄÄNTÄJÄN STANDARDIT JA ANDROID-ASETUKSET
# ====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANDROID)
    message(STATUS "[AI-MOD] Android-ympäristö tunnistettu. Pakotetaan optimoinnit.")
    add_compile_options(-O3 -fPIC -w)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--allow-shlib-undefined")
endif()

# ====================================================================
# 4. MODIN RAKENTAMINEN
# ====================================================================
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
