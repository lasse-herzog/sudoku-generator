#include <stdio.h>
#include <string.h>
#include <ctype.h>

int sudoku_grid[9][9];
char username[20];

/**
 * Prints the active sudoku to the console including row and column description and the username of the current player
 * stored in username.
 */
void PrintSudoku() {
  printf("Player: %s\n", username);
  printf("  abc def ghi\n");
  printf("  --- --- ---\n");

  for (int row = 0; row < 9; ++row) {
    printf("%d|", row + 1);

    for (int column = 0; column < 9; ++column) {
      int current_digit = sudoku_grid[row][column];
      current_digit ? printf("%d", current_digit) : printf(" ");

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
 * @param row_start row index of top-left corner of subgrid to check
 * @param column_start column index of top-left corner of subgrid to check
 * @param digit digit to check presence in subgrid for
 * @return 1 if subgrid doesn't contain digit
 * @return 0 if subgrid contains digit
 */
int NotInSubgrid(int row_start, int column_start, int digit) {
  for (int row_index = row_start; row_index < row_start + 3; row_index++) {
    for (int column_index = column_start; column_index < column_start + 3; column_index++) {
      if (sudoku_grid[row_index][column_index] == digit) {
        return 0;
      }
    }
  }

  return 1;
}

/**
 * Checks if a row already contains digit.
 * @param row index of row to check
 * @param digit digit to check presence in row for
 * @return 1 if row doesn't contain digit
 * @return 0 if row contains digit
 */
int NotInRow(int row, int digit) {
  for (int column = 0; column < 9; column++) {
    if (sudoku_grid[row][column] == digit) {
      return 0;
    }
  }

  return 1;
}

/**
 * Checks if a column already contains digit.
 * @param column index of column to check
 * @param digit digit to check presence in column for
 * @return 1 if column doesn't contain digit
 * @return 0 if column contains digit
 */
int NotInColumn(int column, int digit) {
  for (int row = 0; row < 9; row++) {
    if (sudoku_grid[row][column] == digit) {
      return 0;
    }
  }

  return 1;
}

/**
 * Checks whether a digit is valid at the position specified by the parameters row and column or not.
 * By the rules of sudoku a digit is only valid if it is not already present in the subgrid, row or column.
 * @param row index of row
 * @param column index of column
 * @param digit digit to check validity for
 * @return 1 if digit is a valid option for this position
 * @return 0 if digit is already present in subgrid, row or column
 */
int DigitValid(int row, int column, int digit) {
  return NotInSubgrid(row - (row % 3), column - (column % 3), digit)
    && NotInRow(row, digit)
    && NotInColumn(column, digit);
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

  if (column < 'a' || 'i' < column || row < 1 || 9 < row || digit < 1 || 9 < digit) {
    return 0;
  }

  int row_index = row - 1;
  int column_index = column - 'a';

  sudoku_grid[row_index][column_index] = digit;

  return 1;
}

int main() {
  printf("Please enter your username: ");
  fgets(username, 20, stdin);
  username[strcspn(username, "\n")] = 0;

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

    if (SetDigit(row, column, digit)) {
      PrintSudoku();
    }
  }
}