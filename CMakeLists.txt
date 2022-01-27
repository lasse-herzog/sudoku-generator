cmake_minimum_required(VERSION 3.21)
project(sudoku_generator C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(sudoku_generator sudoku_game.c sudoku_generator.c sudoku_generator.h sudoku_checks.c sudoku_checks.h)
