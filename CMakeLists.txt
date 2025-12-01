cmake_minimum_required(VERSION 3.10)
project(NeuroNet)

# Configurar C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimizaciones
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Incluir directorios
include_directories(include)

# Ejecutable principal
add_executable(neuronet
    src/main.cpp
)

# Información de compilación
message(STATUS "=================================")
message(STATUS "NeuroNet - Sistema de Grafos")
message(STATUS "=================================")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER}")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "=================================")
