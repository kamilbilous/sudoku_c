cmake_minimum_required(VERSION 3.31)
project(sudoku_c C)

set(CMAKE_C_STANDARD 11)

# Add the headers directory for both the library and executable
include_directories(headers)

# Define the sudoku library
add_library(sudoku
        src/board.c
        src/generator.c
        src/menu.c
)

# Make sure the headers are included when using the sudoku library
target_include_directories(sudoku PUBLIC headers)

# Define the executable
add_executable(run_sudoku src/main.c
        src/save_load.c
        headers/save_load.h)

# Link the sudoku library to the executable
target_link_libraries(run_sudoku PRIVATE sudoku)

# Optionally, if you're using math functions, link the math library
# target_link_libraries(run_sudoku PRIVATE sudoku m)
