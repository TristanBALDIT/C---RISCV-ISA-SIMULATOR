cmake_minimum_required(VERSION 3.29)
project(C___RISCV_ISA_SIMULATOR C)

set(CMAKE_C_STANDARD 11)

add_executable(C___RISCV_ISA_SIMULATOR
    decoder.c
        decoder.h
        main.c)
