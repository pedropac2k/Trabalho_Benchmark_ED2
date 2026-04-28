cmake_minimum_required(VERSION 3.29)
project(trabalhoPratico C)

set(CMAKE_C_STANDARD 11)

add_executable(trabalhoPratico main.c
        benchmark.c
        benchmark.h
        ordenacao.h)

target_link_libraries(trabalhoPratico ${CMAKE_SOURCE_DIR}/lib/libordenacao5.a)