cmake_minimum_required(VERSION 3.29)
project(RISCV_ISA_SIMULATOR C)

# Set C standard and compilation flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Directories
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(TEST_BINARY ${TEST_DIR}/addlarge.bin)
set(EXPECTED_OUTPUT ${TEST_DIR}/addlarge-answer.res)
set(ACTUAL_OUTPUT ${TEST_DIR}/addlarge.res)

# Source files
file(GLOB SRC_FILES ${SRC_DIR}/*.c)

# Main executable
add_executable(riscv_sim ${CMAKE_CURRENT_SOURCE_DIR}/main.c ${SRC_FILES})
target_include_directories(riscv_sim PRIVATE ${INCLUDE_DIR})

# Enable testing
enable_testing()

# Add a single CTest test that runs simulator and compares output
add_test(
        NAME compare_test
        COMMAND ${CMAKE_COMMAND} -E env
        ${CMAKE_CURRENT_BINARY_DIR}/riscv_sim ${TEST_BINARY} &&
        ${CMAKE_COMMAND} -E compare_files ${ACTUAL_OUTPUT} ${EXPECTED_OUTPUT}
)

add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_CURRENT_BINARY_DIR}/riscv_sim.exe
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/obj
        COMMAND ${CMAKE_COMMAND} -E rm -f ${TEST_DIR}/*-answer.res
        COMMENT "Cleaning all build artifacts and test outputs..."
)



