#include <stdio.h>
#include <string.h>

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

int main() {
  printf("Please enter your username: ");
  fgets(username, 20, stdin);
  username[strcspn(username, "\n")] = 0;

  PrintSudoku();
}