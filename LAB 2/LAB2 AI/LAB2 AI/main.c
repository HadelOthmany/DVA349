#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


static int test_constraint(int items[9][9], int i, int j, int num)
{
    for (int k = 0; k < 9; k++) {
        if (items[i][k] == num) return 1;   // rad
        if (items[k][j] == num) return 1;   // kolumn
    }

    int sub_grid_i = (i / 3) * 3;
    int sub_grid_j = (j / 3) * 3;
    for (int r = sub_grid_i; r < sub_grid_i + 3; r++)
        for (int c = sub_grid_j; c < sub_grid_j + 3; c++)
            if (items[r][c] == num) return 1;

    return 0;
}

static int backtracking(int items[9][9])
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (items[i][j] == 0) {
                for (int k = 1; k <= 9; k++) {
                    if (test_constraint(items, i, j, k) == 0) {
                        items[i][j] = k;

                        if (backtracking(items)) return 1; // fortsätt

                        items[i][j] = 0; // backtrack
                    }
                }
                return 0; // ingen siffra passar här
            }
        }
    }
    return 1; 
}


static void printSudoku(int items[9][9]) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0 && i != 0) printf("---------------------\n");
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 && j != 0) printf("| ");
            if (items[i][j] == 0) printf(". ");
            else printf("%d ", items[i][j]);
        }
        printf("\n");
    }
}


int main(void) {
    FILE* f = fopen("Assignment 2 sudoku.txt", "r");
    if (!f) { perror("fopen"); return 1; }

    int items[9][9];
    char line[256];
    int num = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "SUDOKU", 6) == 0) {
            num++;

            for (int r = 0; r < 9; r++) {
                fgets(line, sizeof(line), f);
                line[strcspn(line, "\r\n")] = 0;          // ta bort newline
                for (int c = 0; c < 9; c++)
                    items[r][c] = line[c] - '0';         
            }

            printf("\n SUDOKU %d Unsolved \n", num);
            printSudoku(items);
            if (backtracking(items)) {
                printf("\nSUDOKU %d Solved \n", num);
                printSudoku(items);
            }
            else {
                printf("\nSUDOKU %d: No solution \n", num);
            }


        }
    }

    fclose(f);
    return 0;
}


