#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "sudoku_generator.h"

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

int mutability_map[9][9];
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
      int current_digit = sudoku[row][column];

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

  sudoku[row_index][column_index] = digit;

  return 1;
}

/**
 *  Initialize Mutability Map for the player grid in order to keep track of which digits can be changed
 *  and which cannot.
 */
void InitializeMutabilityMap() {
  for (int row = 0; row < 9; ++row) {
    for (int column = 0; column < 9; ++column) {
      if (0 == sudoku[row][column]) {
        mutability_map[row][column] = 1;
      }
    }
  }
}

int CheckIfFinished() {
  for (int row = 0; row < 9; ++row) {
    for (int column = 0; column < 9; ++column) {
      if (sudoku[row][column] != sudoku_solution[row][column]) {
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