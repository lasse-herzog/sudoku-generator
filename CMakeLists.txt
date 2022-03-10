cmake_minimum_required(VERSION 3.21)
project(sudoku-generator C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)
include_directories(. ${CURSES_INCLUDE_DIR})

add_executable(sudoku src/main.c src/sudoku_generator.c src/sudoku_generator.h src/sudoku_checks.c src/sudoku_checks.h src/sudoku_utils.c src/sudoku_utils.h)
target_link_libraries(sudoku ${MENU_LIBRARY} ${CURSES_LIBRARIES})
