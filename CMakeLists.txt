cmake_minimum_required(VERSION 3.29)
project(RISCV_ISA_SIMULATOR C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Répertoires
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Fichiers sources
file(GLOB SRC_FILES ${SRC_DIR}/*.c)

# Exécutable principal
add_executable(riscv_sim main.c ${SRC_FILES})
target_include_directories(riscv_sim PRIVATE ${INCLUDE_DIR})

# Cible de test (équivalent de "make test")
add_custom_target(
        test
        COMMAND riscv_sim ${CMAKE_SOURCE_DIR}/test/addlarge.bin
        COMMAND ${CMAKE_COMMAND} -E echo "Comparing register contents..."
        COMMAND ${CMAKE_COMMAND} -E compare_files
        ${CMAKE_SOURCE_DIR}/test/addlarge.res
        ${CMAKE_SOURCE_DIR}/test/addlarge-answer.res
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS riscv_sim
)

add_custom_target(
        clean-all
        COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/riscv_sim
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/obj
        COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_SOURCE_DIR}/test/*-answer.res
        COMMENT "Cleaning all build artifacts and test outputs..."
)
