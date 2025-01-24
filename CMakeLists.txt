cmake_minimum_required(VERSION 3.10)

project(Kamil_Gebala_SO_Projekt_16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
        init.cpp
        pracownik_dol.cpp
        pracownik_gora.cpp
        krzeslo.cpp
        narciarz.cpp
        kasjer.cpp
        turysta.cpp
)

add_executable(start init.cpp common.h)
add_executable(pracownik_dol pracownik_dol.cpp common.h)
add_executable(pracownik_gora pracownik_gora.cpp common.h)
add_executable(krzeslo krzeslo.cpp common.h)
add_executable(narciarz narciarz.cpp common.h)
add_executable(kasjer kasjer.cpp common.h)
add_executable(turysta turysta.cpp common.h)
