#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

int mutability_map[9][9];
int sudoku_grid[9][9];
int sudoku_solution[9][9];
char username[20];

/**
 * Prints the sudoku represented sudoku_grid to the console including row and column description and the username of the
 * current player stored in username. Digits input by the player are displayed green.
 */
void PrintSudoku() {
  printf("Player: %s\n", username);
  printf("  abc def ghi\n");
  printf("  --- --- ---\n");

  for (int row = 0; row < 9; ++row) {
    printf("%d|", row + 1);

    for (int column = 0; column < 9; ++column) {
      int current_digit = sudoku_grid[row][column];

      if (current_digit) {
        // immutable digits are displayed white, player input green
        mutability_map[row][column] ? printf("%s%d%s", COLOR_GREEN, current_digit, COLOR_RESET)
            : printf("%d", current_digit);
      } else {
        printf(" ");
      }

      if (!((column + 1) % 3)) {
        printf("%c", '|');
      }
    }
    printf("\n");

    if (!((row + 1) % 3)) {
      printf("  --- --- ---\n");
    }
  }
}

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

/**
 * Sets digit at the position specified by the row and column parameters. Returns 0 if one of the parameters was faulty
 * or 1 if the digit was set successfully.
 * @param row index of row as it is displayed on screen, range: 1-9
 * @param column index of column in form of a char as it is displayed on screen, range a-i
 * @param digit digit to be assigned to this position
 * @return 1 if the digit was set successfully
 * @return 0 if either row or column index is out of bounds
 */
int SetDigit(int row, char column, int digit) {
  column = (char) tolower(column);

  int row_index = row - 1;
  int column_index = column - 'a';

  if (!mutability_map[row_index][column_index] || column < 'a' || 'i' < column || row < 1 || 9 < row || digit < 1
      || 9 < digit) {
    return 0;
  }

  sudoku_grid[row_index][column_index] = digit;

  return 1;
}

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
  while (amount != 0) {
    int cell_id = rand() % 80;

    int row = cell_id / 9;
    int column = cell_id % 9;

    if (sudoku_grid[row][column] != 0) {
      sudoku_grid[row][column] = 0;
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

  // copy the values from the solution to the actual sudoku grid before removing digits
  for (int row = 0; row < 9; ++row) {
    for (int column = 0; column < 9; ++column) {
      sudoku_grid[row][column] = sudoku_solution[row][column];
    }
  }

  RemoveDigits(1);
}

/**
 *  Initialize Mutability Map for the player grid in order to keep track of which digits can be changed
 *  and which cannot.
 */
void InitializeMutabilityMap() {
  for (int row = 0; row < 9; ++row) {
    for (int column = 0; column < 9; ++column) {
      if (0 == sudoku_grid[row][column]) {
        mutability_map[row][column] = 1;
      }
    }
  }
}

int CheckIfFinished() {
  for (int row = 0; row < 9; ++row) {
    for (int column = 0; column < 9; ++column) {
      if (sudoku_grid[row][column] != sudoku_solution[row][column]) {
        return 0;
      }
    }
  }

  return 1;
}

int main() {
  printf("Please enter your username: ");
  fgets(username, 20, stdin);
  username[strcspn(username, "\n")] = 0;

  GenerateSudoku();

  InitializeMutabilityMap();

  PrintSudoku();

  int running = 1;
  int row;
  char column;
  int digit;

  while (running) {
    printf("Please enter a field (input format: [column][row], e.g. a5): ");
    scanf("%c%d", &column, &row);
    printf("Enter digit: ");
    scanf("%d", &digit);

    if (!SetDigit(row, column, digit)) {
      continue;
    }

    PrintSudoku();
    if (CheckIfFinished()) {
      running = 0;
    };
  }
}