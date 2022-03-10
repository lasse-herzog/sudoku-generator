#include <string.h>
#include <menu.h>
#include <stdlib.h>
#include "sudoku_generator.h"

WINDOW *sudoku_window;
WINDOW *menu_window;

MENU *main_menu;
MENU *game_menu;

ITEM **main_menu_items;
ITEM **game_menu_items;

char username[20];

int CheckIfFinished();

void CleanupBoard();

void InitBoard();

void InitNcurses();

void InputLoop();

void PrintSudoku();

int SetDigit(int row, int column, int digit);

int main(void) {
  int input;

  InitNcurses();

  main_menu_items = (ITEM **) calloc(3, sizeof(ITEM *));
  main_menu_items[0] = new_item("Start Game", "");
  main_menu_items[1] = new_item("   Quit   ", "");
  main_menu_items[2] = 0;

  main_menu = new_menu(main_menu_items);
  post_menu(main_menu);

  while ((input = getch()) != KEY_F(1)) {
    switch (input) {
      case KEY_DOWN:
        menu_driver(main_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(main_menu, REQ_UP_ITEM);
        break;
      case 0xA:
        switch (item_index(current_item(main_menu))) {
          case 0:
            unpost_menu(main_menu);

            InitBoard();
            break;
          case 1:
            // TODO correctly configure quit
            exit(0);
        }
        break;
      default:
        break;
    }
  }

  return (0);
}
void InitNcurses() {
  initscr();
  cbreak();
  atexit(CleanupBoard);

  noecho();
  curs_set(0);

  nl();
  keypad(stdscr, TRUE);

  if (has_colors()) {
    start_color();
    use_default_colors();

    init_pair(1, 231, -1);
    // purple
    init_pair(2, 183, -1);
    // yellow
    init_pair(3, 229, -1);
    // pink
    init_pair(4, 212, -1);
  }
}

/**
 * Prints the sudoku represented sudoku_grid to the console including row and column description and the username of the
 * current player stored in username. Digits input by the player are displayed green.
 */
void PrintSudoku() {
  char character;
  int digit;

  wattron(sudoku_window, COLOR_PAIR(4));
  wattron(sudoku_window, A_BOLD);

  wmove(sudoku_window, 0, 0);
  wprintw(sudoku_window, "Player: %s\n", username);

  wattron(sudoku_window, COLOR_PAIR(1));

  for (int row = 0; row <= 18; ++row) {
    for (int column = 0; column <= 36; ++column) {
      bool empty = false;

      if (column % 12 == 0 || row % 6 == 0) {
        wattroff(sudoku_window, A_DIM);
      } else {
        wattron(sudoku_window, A_DIM);
      }

      if (row % 2) {
        if (column % 4 == 0) {
          character = ACS_VLINE;
        } else if (column % 2 == 0) {
          digit = sudoku[row / 2][column / 4];

          if (0 == digit) {
            empty = true;
          } else {
            character = (char) (digit + '0');
          }
        } else {
          empty = true;
        }
      } else if (column % 2) {
        character = ACS_HLINE;
      } else {
        if (0 == row) {
          if (0 == column) {
            character = ACS_ULCORNER;
          } else if (36 == column) {
            character = ACS_URCORNER;
          } else if (column % 4 == 0) {
            character = ACS_TTEE;
          } else {
            character = ACS_HLINE;
          }
        } else if (18 == row) {
          if (0 == column) {
            character = ACS_LLCORNER;
          } else if (36 == column) {
            character = ACS_LRCORNER;
          } else if (column % 4 == 0) {
            character = ACS_BTEE;
          } else {
            character = ACS_HLINE;
          }
        } else {
          if (0 == column) {
            character = ACS_LTEE;
          } else if (36 == column) {
            character = ACS_RTEE;
          } else if (column % 4 == 0) {
            character = ACS_PLUS;
          } else {
            character = ACS_HLINE;
          }
        }
      }

      if (empty) {
        waddch(sudoku_window, ' ');
      } else {
        if ('1' <= character && character <= '9') {
          wattron(sudoku_window, COLOR_PAIR(2));
          wattroff(sudoku_window, A_DIM);
          waddch(sudoku_window, character);
          wattroff(sudoku_window, COLOR_PAIR(2));
        } else {
          waddch(sudoku_window, NCURSES_ACS(character));
        }
      }
    }
  }
}

/**
 * Sets digit at the position specified by the row and column parameters. Returns 0 if one of the parameters was faulty
 * or 1 if the digit was set successfully.
 * @param row index of row as main_menu_items is displayed on screen, range: 1-9
 * @param column index of column in form of a char as main_menu_items is displayed on screen, range a-i
 * @param digit digit to be assigned to this position
 * @return 1 if the digit was set successfully
 * @return 0 if either row or column index is out of bounds
 */
int SetDigit(int row, int column, int digit) {
  if (!mutability_map[row][column]) {
    return 0;
  }

  sudoku[row][column] = digit - '0';
  wattron(sudoku_window, COLOR_PAIR(3));
  waddch(sudoku_window, digit);
  wattroff(sudoku_window, COLOR_PAIR(3));

  return 1;
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

void CleanupBoard(void) {
  int i;

  free_menu(game_menu);

  for (i = 0; i <= 4; i++) {
    free_item(game_menu_items[i]);
  }

  free(game_menu_items);

  delwin(sudoku_window);
  delwin(menu_window);
}

int is_running;
int x = 2;
int y = 2;

void InitBoard() {
  is_running = 1;

  sudoku_window = newwin(20, 37, 0, 0);
  menu_window = newwin(20, 20, 0, 50);

  wattron(menu_window, COLOR_PAIR(1));
  box(menu_window, 0, 0);

  game_menu_items = (ITEM **) calloc(4, sizeof(ITEM *));
  game_menu_items[0] = new_item("d", "Difficulty");
  game_menu_items[1] = new_item("r", "Reroll");
  game_menu_items[2] = new_item("q", "Main Menu");
  game_menu_items[3] = 0;

  game_menu = new_menu(game_menu_items);
  set_menu_win(game_menu, menu_window);
  set_menu_sub(game_menu, derwin(menu_window, 16, 16, 1, 2));
  post_menu(game_menu);

  GenerateSudoku();
  PrintSudoku();

  wmove(sudoku_window, y, x);
  curs_set(1);

  refresh();
  wrefresh(menu_window);
  wrefresh(sudoku_window);

  InputLoop();

  CleanupBoard();
  post_menu(main_menu);

  refresh();
}

void InputLoop() {
  int input;
  bool in_game = true;


  while (is_running && (input = getch()) != KEY_F(1)) {
    switch (input) {
      case KEY_DOWN:
        if (in_game) {
          if (y < 18) {
            y += 2;
          }
        } else {
          menu_driver(game_menu, REQ_DOWN_ITEM);
        }
        break;
      case KEY_UP:
        if (in_game) {
          if (3 < y) {
            y -= 2;
          }
        } else {
          menu_driver(game_menu, REQ_UP_ITEM);
        }
        break;
      case KEY_RIGHT:
        if (in_game && x < 34) {
          x += 4;
        }
        break;
      case KEY_LEFT:
        if (in_game && 3 < x) {
          x -= 4;
        }
        break;
      case 'm':
        in_game = !in_game;
        break;
      case '1' ... '9':
        if (in_game && SetDigit((y - 2) / 2, (x - 2) / 4, input)) {
          if (CheckIfFinished()) {
            is_running = 0;
          }
        }
        break;
      case 0xA:
        if (in_game) {
          return;
        }

        switch (item_index(current_item(game_menu))) {
          case 0:
            break;
          case 1:
            GenerateSudoku();
            PrintSudoku();

            wrefresh(sudoku_window);
            break;
          case 2:
            is_running = false;
            break;
        }
        break;
    }

    if (in_game) {
      curs_set(1);
      wmove(sudoku_window, y, x);
      wrefresh(sudoku_window);
    } else {
      curs_set(0);
      wrefresh(menu_window);
    }
  }
}
