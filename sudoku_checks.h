#ifndef SUDOKU_GENERATOR__SUDOKU_CHECKS_H_
#define SUDOKU_GENERATOR__SUDOKU_CHECKS_H_

extern int DigitValid(int sudoku[9][9], int row, int column, int digit);

extern int NotInColumn(int sudoku[9][9], int column, int digit);

extern int NotInRow(int sudoku[9][9], int row, int digit);

extern int NotInSubgrid(int sudoku[9][9], int row_start, int column_start, int digit);

#endif //SUDOKU_GENERATOR__SUDOKU_CHECKS_H_
