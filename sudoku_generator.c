#include <stdlib.h>
#include <time.h>
#include "sudoku_checks.h"
#include "sudoku_generator.h"

int sudoku[9][9];
int sudoku_solution[9][9];

/**
 * Function to fill the diagonal subgrids (top-left, middle, bottom-right) by iterating through them and filling them
 * with pseudo-random generated numbers.
 */
void FillDiagonalSubgrids() {
  int digit;

  for (int subgrid_top_left = 0; subgrid_top_left < 9; subgrid_top_left += 3) {
    for (int row_index = subgrid_top_left; row_index < subgrid_top_left + 3; ++row_index) {
      for (int column_index = subgrid_top_left; column_index < subgrid_top_left + 3; ++column_index) {
        do {
          digit = (rand() % 9) + 1;
        } while (!NotInSubgrid(sudoku_solution, subgrid_top_left, subgrid_top_left, digit));

        sudoku_solution[row_index][column_index] = digit;
      }
    }
  }
}

/**
 * This function fills all the missing values which are not in one of the diagonal subgrids
 * (top-left, middle, bottom-right). It uses an recursive approach to check if a digit is not only valid for a specific
 * position at the current time but also if it is a valid option for positions which are still empty.
 * @param row_index row index
 * @param column_index column index
 * @return 1 if the sudoku is completely filled or if all levels of recursion below returned 1.
 * @return 0 if no digit matches the criteria for this position.
 */
int FillRemainingPositions(int row_index, int column_index) {
  // moving to the next row if last column has been reached
  if (column_index == 9) {
    column_index = 0;
    ++row_index;
  }

  // skipping the diagonal subgrids (top-left, middle, bottom-right) as they are already filled
  if (column_index < 3 && row_index < 3) {
    column_index = 3;
  } else if (column_index == 3 && 3 <= row_index && row_index < 6) {
    column_index += 3;
  } else if (column_index == 6 && 6 <= row_index) {
    ++row_index;
    column_index = 0;
  }

  // stopping criterion, all positions of the sudoku grid have been filled
  if (row_index >= 9) {
    return 1;
  }

  for (int digit = 1; digit <= 9; digit++) {
    if (DigitValid(sudoku_solution, row_index, column_index, digit)) {
      sudoku_solution[row_index][column_index] = digit;

      // entering next recursion level
      if (FillRemainingPositions(row_index, column_index + 1)) {
        return 1;
      }

      // digit does not fit in this position
      // resetting the digit at this position to 0 as checks would get messed up otherwise
      sudoku_solution[row_index][column_index] = 0;
    }
  }

  return 0;
}

/**
 * This Function removes digits from the generated Sudoku so that it becomes an actual challenge for
 * the player ;).
 * @param amount amount of digits to be removed from the grid, the more, the higher the difficulty of the sudoku
 */
void RemoveDigits(int amount) {
  // copy the values from the solution to the actual sudoku grid before removing digits
  for (int row = 0; row < 9; ++row) {
    for (int column = 0; column < 9; ++column) {
      sudoku[row][column] = sudoku_solution[row][column];
    }
  }

  while (amount != 0) {
    int cell_id = rand() % 80;

    int row = cell_id / 9;
    int column = cell_id % 9;

    if (sudoku[row][column] != 0) {
      sudoku[row][column] = 0;
      amount--;
    }
  }
}

/**
 * Generates a Sudoku by first filling 3 diagonal subgrids and than recursively filling the rest of the grid.
 * Once finished depending on the chosen difficulty a certain number of digits will be removed from the grid.
 */
void GenerateSudoku() {
  srand(time(NULL));

  FillDiagonalSubgrids();
  FillRemainingPositions(0, 3);
  RemoveDigits(1);
}