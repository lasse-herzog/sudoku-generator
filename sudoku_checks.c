#include "sudoku_checks.h"

/**
 * Checks if a subgrid already contains digit.
 * @param sudoku the sudoku grid in which the check will be performed
 * @param row_start row index of top-left corner of subgrid to check
 * @param column_start column index of top-left corner of subgrid to check
 * @param digit digit to check presence in subgrid for
 * @return 1 if subgrid doesn't contain digit
 * @return 0 if subgrid contains digit
 */
int NotInSubgrid(int sudoku[9][9], int row_start, int column_start, int digit) {
  for (int row_index = row_start; row_index < row_start + 3; row_index++) {
    for (int column_index = column_start; column_index < column_start + 3; column_index++) {
      if (sudoku[row_index][column_index] == digit) {
        return 0;
      }
    }
  }

  return 1;
}

/**
 * Checks if a row already contains digit.
 * @param sudoku the sudoku grid in which the check will be performed
 * @param row index of row to check
 * @param digit digit to check presence in row for
 * @return 1 if row doesn't contain digit
 * @return 0 if row contains digit
 */
int NotInRow(int sudoku[9][9], int row, int digit) {
  for (int column = 0; column < 9; column++) {
    if (sudoku[row][column] == digit) {
      return 0;
    }
  }

  return 1;
}

/**
 * Checks if a column already contains digit.
 * @param sudoku the sudoku grid in which the check will be performed
 * @param column index of column to check
 * @param digit digit to check presence in column for
 * @return 1 if column doesn't contain digit
 * @return 0 if column contains digit
 */
int NotInColumn(int sudoku[9][9], int column, int digit) {
  for (int row = 0; row < 9; row++) {
    if (sudoku[row][column] == digit) {
      return 0;
    }
  }

  return 1;
}

/**
 * Checks whether a digit is valid at the position specified by the parameters row and column or not.
 * By the rules of sudoku a digit is only valid if it is not already present in the subgrid, row or column.
 * @param sudoku the sudoku grid in which the check will be performed
 * @param row index of row
 * @param column index of column
 * @param digit digit to check validity for
 * @return 1 if digit is a valid option for this position
 * @return 0 if digit is already present in subgrid, row or column
 */
int DigitValid(int sudoku[9][9], int row, int column, int digit) {
  return NotInSubgrid(sudoku, row - (row % 3), column - (column % 3), digit)
      && NotInRow(sudoku, row, digit)
      && NotInColumn(sudoku, column, digit);
}