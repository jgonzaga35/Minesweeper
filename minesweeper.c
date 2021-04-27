// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Justin Gonzaga 
// on 14-03-2020

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0 // a square that has not been revealed.
#define HIDDEN_SAFE     1 // a hidden square that does not contain a mine.
#define HIDDEN_MINE     2 // a square that has not been revealed and contains a mine.

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.
#define SQUARE_LENGTH           3
#define MAX_HINTS               3
#define GAME_RUNNING            99
#define GAME_OVER               100
#define DASH_NUMBER             25
#define OUT_OF_BOUNDS           40
#define RIGHT                   1
#define TOP_RIGHT               2
#define UP                      3
#define TOP_LEFT                4
#define LEFT                    5
#define BOTTOM_LEFT             6
#define DOWN                    7
#define BOTTOM_RIGHT            8
#define FIRST_TURN              1
#define NO_MINES                0
#define HASH                    49
#define PARANTHESES             50     
#define MINES_NEARBY            2         

// Declare functions.
void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void place_mines(int minefield[SIZE][SIZE]);
void detect_row(int hintNumber, int minefield[SIZE][SIZE]);
void detect_column(int hintNumber, int minefield[SIZE][SIZE]);
void detect_square(int hintNumber, int minefield[SIZE][SIZE]);
void reveal_square(int turn, int mode, int minefield[SIZE][SIZE]);
void reveal_radial(int turn, int mode, int minefield[SIZE][SIZE]);
void shift_grid(int minefield[SIZE][SIZE]);
void convert_num(int row, int column, int minefield[SIZE][SIZE]);
int count_mines(int row, int column, int minefield[SIZE][SIZE]);
int detect_mines(int row, int column, int minefield[SIZE][SIZE]);
void convert_safe(int row, int column, int minefield[SIZE][SIZE]);
void convert_square(int row, int column, int minefield[SIZE][SIZE]);
void reveal_direction (int row, int column, int minefield[SIZE][SIZE]);
void print_game_minefield(int stage, int minefield[SIZE][SIZE]);
void print_game_overlay(int type, int minefield[SIZE][SIZE]);
void detect_win(int mode, int minefield[SIZE][SIZE]);

int main(void) {
    // Set all mines to HIDDEN_SAFE. 
    // Prompt user to enter number of mines and mine locations.
    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");
    place_mines(minefield);

    printf("Game Started\n");
    print_debug_minefield(minefield);

    // Declare and initialise default game settings.
    int scannedNum;
    int hintNumber = 0; 
    int mode = DEBUG_MODE;
    int turn = FIRST_TURN;
    // Scan in commands to play the game until the game ends.
    while (scanf("%d", &scannedNum) == 1) {
        // Stage 1 Commands
        if (scannedNum == DETECT_ROW) { 
            detect_row(hintNumber, minefield);
            hintNumber++;       
        } else if (scannedNum == DETECT_COL) {
            detect_column(hintNumber, minefield);
            hintNumber++;
        }
        // Stage 2 Commands
        if (scannedNum == DETECT_SQUARE) {
            detect_square(hintNumber, minefield);
            hintNumber++;
        } else if (scannedNum == REVEAL_SQUARE) {
            reveal_square(turn, mode, minefield);
            turn++;
        } 
        // Stage 3 Commands
        if (scannedNum == DEBUG_MODE) {
            printf("Debug mode activated\n");
            mode = DEBUG_MODE;
        } else if (scannedNum == GAMEPLAY_MODE) {
            printf("Gameplay mode activated\n");
            mode = GAMEPLAY_MODE; 
        } 
        // Stage 4 Commands
        if (scannedNum == REVEAL_RADIAL) {
            reveal_radial(turn, mode, minefield);
            turn++;
        }         
        // Display appropriate minefield after each command has been processed.
        if (mode == DEBUG_MODE || scannedNum == DEBUG_MODE) {
            print_debug_minefield(minefield);
        } else if (mode == GAMEPLAY_MODE || scannedNum == GAMEPLAY_MODE) {
            print_game_minefield(GAME_RUNNING, minefield);
        }
    } 
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    return 0;
}
// Stage 1
// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}
// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}
// Scan coordinates and place mines onto grid.
void place_mines(int minefield[SIZE][SIZE]) {
    // Scan in number of pairs of mines from standard input.
    int pairNum = 0;
    scanf("%d", &pairNum);

    // Prompt user to enter location of each mine from standard input.
    printf("Enter pairs:\n");
    int i = 0;
    while (i < pairNum) { 
        int row;
        int column;
        scanf("%d %d", &row, &column);
        if (row >= SIZE || column < 0) { 
        } else if (row < 0 || column >= SIZE) { 
        } else {
            minefield[row][column] = HIDDEN_MINE; 
        }
        i++;
    } 
}
// Check a specified column for mines and return number of mines to user.
void detect_column(int hintNumber, int minefield[SIZE][SIZE]) {
    int column = 0;
    int row = 0;
    int mineNumber = 0;
    scanf("%d", &column);
    if (hintNumber < MAX_HINTS) {
        while (row < SIZE) {
            if (minefield[row][column] == HIDDEN_MINE) {
                mineNumber++;
            } 
            row++;
        }      
        printf("There are %d mine(s) in column %d\n", mineNumber, column);
    } else if (hintNumber >= MAX_HINTS) {
        printf("Help already used\n");
    }
}
// Check a specified row for mines and return number of mines to user.
void detect_row(int hintNumber, int minefield[SIZE][SIZE]) {
    int row = 0;
    int column = 0;
    int mineNumber = 0;
    scanf("%d", &row);
    
    if (hintNumber < MAX_HINTS) {
        while (column < SIZE) {
            if (minefield[row][column] == HIDDEN_MINE) {
                mineNumber++;
            } 
            column++;
        }
        printf("There are %d mine(s) in row %d\n", mineNumber, row);
    } else if (hintNumber >= MAX_HINTS) {
        printf("Help already used\n");
    }
}
// Stage 2
// Check a specified square for mines and return number of mines to user.
void detect_square(int hintNumber, int minefield[SIZE][SIZE]) {
    int row;
    int column;
    int size; 
    scanf("%d %d %d", &row, &column, &size);

    if (hintNumber < MAX_HINTS) {
        printf("There are %d mine(s) in the square", 
        count_mines(row, column, minefield)); 
        printf(" centered at row %d, column %d of size %d\n", row, column, size); 
    } else if (hintNumber >= MAX_HINTS) {
        printf("Help already used\n");
    }
}
// Reveal a specified square of length SQUARE_LENGTH. 
void reveal_square(int turn, int mode, int minefield[SIZE][SIZE]) {
    int row;
    int column;
    scanf("%d %d", &row, &column);

    // Shift the grid down one row if a HIDDEN_MINE is selected on the first turn.
    if (minefield[row][column] == HIDDEN_MINE && turn == FIRST_TURN) {
        shift_grid(minefield);
        turn++;
        convert_safe(row, column, minefield);
    } 

    // End the game if a HIDDEN_MINE is selected after the first turn.
    else if (minefield[row][column] == HIDDEN_MINE && mode == GAMEPLAY_MODE) {
        printf("Game over\n");
        printf("xx\n");
        printf("/\\");
        printf("\n");
        print_game_minefield(GAME_OVER, minefield);
        exit (EXIT_FAILURE); 
    } else if (minefield[row][column] == HIDDEN_MINE && mode == DEBUG_MODE) {
        printf("Game over\n");
        print_debug_minefield(minefield);
        exit (EXIT_FAILURE);
    } 

    // Reveal eligible squares if a HIDDEN_SAFE is selected.
    else { 
        convert_safe(row, column, minefield);
    }
    detect_win(mode, minefield);
}
// Count the number of mines in a square. 
// If type is REVEAL, convert selected square to VISIBLE_SAFE.
int detect_mines (int row, int column, int minefield[SIZE][SIZE]) {
    // Ignore input if it is out of bounds.
    if (row < 0 || column < 0) {
        return OUT_OF_BOUNDS;
    } else if (row >= SIZE || column >= SIZE) {
        return OUT_OF_BOUNDS;
    } 
    
    // Return a number representing if there are adjacent mines or not.
    if (count_mines(row, column, minefield) > 0) {
        minefield[row][column] = VISIBLE_SAFE;
        return MINES_NEARBY;
    } else if (count_mines(row, column, minefield) == 0) {
        minefield[row][column] = VISIBLE_SAFE;
        return NO_MINES;
    }
    return 42;
}
// Count number of mines adjacent to a square.
int count_mines(int row, int column, int minefield[SIZE][SIZE]) {
    int length = SQUARE_LENGTH;
    int i = 0;
    int j = 0;
    int x = row - ((length - 1)/2);
    int y = column - ((length - 1)/2);
    int mineNumber = 0;

    // Go through rows for the height of the square.
    while (i < length) {
        // Go across columns for the length of the square.
        y = column - ((length - 1)/2); 
        j = 0; 
        while (j < length) {
            if (x < 0 || y < 0) {
            } else if (x >= SIZE || y >= SIZE) {
            } else if (minefield[x][y] == HIDDEN_MINE) {
                mineNumber++;
            }
            y++;
            j++;
        }
        x++;
        i++;
    }
    return mineNumber;
}

// Convert eligible HIDDEN_SAFE squares in a given square into VISIBLE_SAFE.
void convert_safe(int row, int column, int minefield[SIZE][SIZE]) {
    // Convert the selected square if there are adjacent mines.
    if (count_mines(row, column, minefield) > NO_MINES) {
        minefield[row][column] = VISIBLE_SAFE;    
    } 
    
    // Convert all adjacent squares if there are no adjacent mines.
    else if (count_mines(row, column, minefield) == NO_MINES) {
        convert_square(row, column, minefield);
    }
}
// Convert all elements in a square of length SQUARE_LENGTH into VISIBLE_SAFE.  
void convert_square(int row, int column, int minefield[SIZE][SIZE]) {
    int length = SQUARE_LENGTH;
    int x = row - ((length - 1)/2);
    int y = column - ((length - 1)/2);
    int i = 0;
    int j = 0;    
    
    while (i < SQUARE_LENGTH) {
        while (j < SQUARE_LENGTH) {
            if (x < 0 || y < 0) {
            } else if (x >= SIZE || y >= SIZE) {
            } else if (minefield[x][y] == HIDDEN_SAFE) {
                minefield[x][y] = VISIBLE_SAFE;
            }
            y++;
            j++;
        }
        y = column - ((length -1)/2); 
        j = 0;
        x++; 
        i++;
    }
}
// Stage 3 
// Print gameplay minefield.
void print_game_minefield(int stage, int minefield[SIZE][SIZE]) {
    if (stage == GAME_OVER) {
        print_game_overlay(PARANTHESES, minefield);
        exit(EXIT_FAILURE);
    } else if (stage == GAME_RUNNING) {
        printf("..\n");
        printf("\\/");
        printf("\n");
    }
    print_game_overlay(HASH, minefield);
}
void print_game_overlay(int type, int minefield[SIZE][SIZE]) {
    int row = 0;
    int line = 0;
    // Display game mode overlay.
    while (row < SIZE) {
        int column = 0;
        if (line == 0) {
            printf("    ");
            int i = 0;
            // Display column number indicators.
            while (i < SIZE) {
                printf("%.2d ", i);
                i++;
            }
            printf("\n");
            printf("   ");
            int j = 0;
            while (j < DASH_NUMBER) {
                printf("-");
                j++;
            }
            printf("\n");
        }
        while (column < SIZE) {
            // Display row number indicators.
            if (column == 0) {
                printf("%.2d |", row);
            }
            // Display minefield.
            if (minefield[row][column] == HIDDEN_SAFE) { 
                printf("##");
            } else if (minefield[row][column] == HIDDEN_MINE && type == HASH) {
                printf("##");
            } else if (minefield[row][column] == HIDDEN_MINE && type == PARANTHESES) {
                printf("()");
            } else if (minefield[row][column] == VISIBLE_SAFE) {
                convert_num(row, column, minefield);
            }
            // Print right boundaries.
            if (column == SIZE - 1) {
                printf("|");
            }
            // Print spacing in between elements.
            else if (column < SIZE - 1 && column >= 0) {
                printf(" ");
            }
            column++;
        }
        printf("\n");
        // Print bottom boundaries.
        if (row == SIZE - 1) {
            printf("   ");
            column = 0;
            while (column < DASH_NUMBER) {
                printf("-");
                column++;
            }
            printf("\n");
        }
        row++;
        line++;
    }
}
// Converts VISIBLE_SAFE mine to a number displaying the number of adjacent mines.
void convert_num(int row, int column, int minefield[SIZE][SIZE]) {
    if (count_mines(row, column, minefield) > 0) {
        printf("%.2d", count_mines(row, column, minefield)); 
    } else {
        printf("  ");
    }
}
// Checks number of HIDDEN_SAFE remaining. 
// Ends game if there are no more HIDDEN_SAFE left.
void detect_win(int mode, int minefield[SIZE][SIZE]) {
    int row = 0;
    int column = 0;
    int hiddenNumber = 0;

    // Scan entire grid for HIDDEN_SAFE.
    while (row < SIZE) {
        while (column < SIZE) {
            if (minefield[row][column] == HIDDEN_SAFE) {
                hiddenNumber++;
            }
            column++;
        }
        column = 0;
        row++;
    }
    // Exit game if minefield contains no more HIDDEN_SAFE.
    if (hiddenNumber == 0 && mode == DEBUG_MODE) {
        printf("Game won!\n");
        print_debug_minefield(minefield);
        exit (EXIT_FAILURE);
    } else if (hiddenNumber == 0 && mode == GAMEPLAY_MODE) {
        printf("Game won!\n");
        print_game_minefield(GAME_RUNNING, minefield);
        exit (EXIT_FAILURE);
    }
}
// Stage 4
void reveal_radial(int turn, int mode, int minefield[SIZE][SIZE]) {
    int row;
    int column;
    scanf("%d %d", &row, &column);

    // Shift grid 1 row down if user selects HIDDEN_MINE on the first turn.
    if (minefield[row][column] == HIDDEN_MINE && turn == FIRST_TURN) {
        shift_grid(minefield);
        // Only reveal selected square if there are adjacent mines.
        if (count_mines(row, column, minefield) > NO_MINES) {
            minefield[row][column] = VISIBLE_SAFE;
        } else if (count_mines(row, column, minefield) == NO_MINES) {
            reveal_direction(row, column, minefield);
        }
    } else {
        // End game if a hidden mine is selected.
        if (minefield[row][column] == HIDDEN_MINE && mode == GAMEPLAY_MODE) {
            print_game_minefield(GAME_OVER, minefield);
            // from stackOverflow - exit a program within a void function.
            exit (EXIT_FAILURE); 
        } else if (minefield[row][column] == HIDDEN_MINE && mode == DEBUG_MODE) {
            printf("Game over\n");
            print_debug_minefield(minefield);
            exit (EXIT_FAILURE);
        }
        // Only reveal selected square if there are adjacent mines.
        else if (count_mines(row, column, minefield) > NO_MINES) {
            minefield[row][column] = VISIBLE_SAFE;
        }
        // Else reveal appropriate squares
        else {
            reveal_direction(row, column, minefield);
        }
    }
    detect_win(mode, minefield);
} 
// Reveal squares in all directions as long as there are no adjacent mines.
void reveal_direction (int row, int column, int minefield[SIZE][SIZE]) {
    int x = row;
    int y = column;

    int direction = 1;
    while (direction < 9) {
        if (direction == RIGHT) {
            while (detect_mines(x, y, minefield) == NO_MINES) {
                y++;
            }
        } else if (direction == TOP_RIGHT) {
            while (detect_mines(x, y, minefield) == NO_MINES) {
                x--;
                y++;
            }
        } else if (direction == UP) {
            while (detect_mines(x, y, minefield) == NO_MINES) {
                x--;
            }
        } else if (direction == TOP_LEFT) { 
            while (detect_mines(x, y, minefield) == NO_MINES) {
                x--;
                y--;
            }
        } else if (direction == LEFT) {
            while (detect_mines(x, y, minefield) == NO_MINES) {
                y--;
            }
        } else if (direction == BOTTOM_LEFT) {
            while (detect_mines(x, y, minefield) == NO_MINES) {
                x++;
                y--;
            }
        } else if (direction == DOWN) { 
            while (detect_mines(x, y, minefield) == NO_MINES) {
                x++;
            }
        } else if (direction == BOTTOM_RIGHT) { 
            while (detect_mines(x, y, minefield) == NO_MINES) {
                x++;
                y++;
            }
        }
        y = column;
        x = row;
        direction++;
    }
}
void shift_grid(int minefield[SIZE][SIZE]) {
    int temp[SIZE][SIZE];
    int row = SIZE - 1;
    int column = 0;
    int lastRow = 0;

    // Shift the grid 1 row down.
    while (row >= 0) {
        column = 0;
        while (column < SIZE) {
            if (row == SIZE - 1) {
                // Store last row into a temporary array.
                temp[lastRow][column] = minefield[row][column];
                // Shift second last row into last row.
                minefield[row][column] = minefield[row - 1][column];
            } else if (row > 0 && row < SIZE - 1) { 
                minefield[row][column] = minefield[row - 1][column];
            } else if (row == 0) {
                minefield[row][column] = temp[lastRow][column];
            }
            column++;
        }
        row--;
    }
}
