cmake_minimum_required(VERSION 3.25)
project(AIEditorMod VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "[AI-MOD] ALOITETAAN JÄRETÖN GEODE-SÄÄNTÖJEN OHITUS...")

# ====================================================================
# 1. LUODAAN OMA INTERNETTIÄ TAI SDK-LATAUKSIA VAATIMATON VALE-GEODE JÄRJESTELMÄ
# ====================================================================
set(Geode_FOUND TRUE)
set(Geode_VERSION 3.0.0)

# Korvataan Geoden virallinen add_geode_mod-komento omalla, joka tekee suoraan Android-kirjaston
macro(add_geode_mod MOD_NAME)
    message(STATUS "[AI-MOD] Kaapataan komento add_geode_mod kohteelle: ${MOD_NAME}")
    
    # Haetaan kaikki SOURCES-sanan jälkeen annetut tiedostot
    set(MULTI_VAL ${ARGN})
    list(REMOVE_ITEM MULTI_VAL SOURCES)
    
    # Luodaan suoraan jaettu Android-kirjasto (.so), jonka Geode-lataaja pystyy lukemaan!
    add_library(${MOD_NAME} SHARED ${MULTI_VAL})
    
    # Pakotetaan kääntäjäohjelma antamaan tiedostolle nimeksi se, mitä Geode odottaa
    set_target_properties(${MOD_NAME} PROPERTIES 
        PREFIX ""
        SUFFIX ".so"
        LINKER_LANGUAGE CXX
    )
    
    message(STATUS "[AI-MOD] Vale-Geode on onnistuneesti luonut kohteen: ${MOD_NAME}")
endmacro()

# ====================================================================
# 2. KÄÄNTÄJÄN STANDARDIT JA ANDROID-ASETUKSET
# ====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANDROID)
    message(STATUS "[AI-MOD] Android-ympäristö tunnistettu. Pakotetaan optimoinnit.")
    add_compile_options(-O3 -fPIC -w)
    # Estetään kääntäjää valittamasta puuttuvista Geode-kirjastoista linkitysvaiheessa
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--allow-shlib-undefined")
endif()

# ====================================================================
# 3. MODIN LOPULLINEN RAKENTAMINEN (GEODE LUULEE TÄMÄN OLEVAN VIRALLINEN)
# ====================================================================
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
