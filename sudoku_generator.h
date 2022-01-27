#ifndef SUDOKU_GENERATOR_CMAKE_BUILD_DEBUG_SUDOKU_GENERATOR_H_
#define SUDOKU_GENERATOR_CMAKE_BUILD_DEBUG_SUDOKU_GENERATOR_H_

extern int sudoku[9][9];

extern int sudoku_solution[9][9];

extern void FillDiagonalSubgrids();

extern int FillRemainingPositions(int row_index, int column_index);

extern void GenerateSudoku();

extern void RemoveDigits(int amount);
#endif //SUDOKU_GENERATOR_CMAKE_BUILD_DEBUG_SUDOKU_GENERATOR_H_
