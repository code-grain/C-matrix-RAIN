#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 80
#define HEIGHT 25
#define DELAY_MS 200
#define MAX_TRAIL_LEN 5

void sleep_ms(int milliseconds) {
    Sleep(milliseconds);
}

void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    int i, j, k;
    char matrix_chars[] = "@#$-."; // Bright to dim
    int matrix_chars_len = sizeof(matrix_chars) - 1;
    int *y_pos = (int *)calloc(WIDTH, sizeof(int));
    int *trail_len = (int *)calloc(WIDTH, sizeof(int)); // Variable trail lengths
    char trails[WIDTH][MAX_TRAIL_LEN]; // 2D array for trails
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Initialize positions and trails
    srand(time(NULL));
    for (i = 0; i < WIDTH; i++) {
        y_pos[i] = rand() % HEIGHT;
        trail_len[i] = rand() % MAX_TRAIL_LEN + 1; // Random length 1 to 5
        for (k = 0; k < MAX_TRAIL_LEN; k++) {
            trails[i][k] = (k < trail_len[i]) ? matrix_chars[k % matrix_chars_len] : ' ';
        }
    }

    system("color 0a"); // Base green on black
    char screen[HEIGHT][WIDTH + 1]; // Buffer for screen

    while (1) {
        if (kbhit() && getch() == 'q') break;

        // Update positions and trails
        for (i = 0; i < WIDTH; i++) {
            if (rand() % 100 < 80) { // 80% chance to move
                y_pos[i]++;
                for (k = trail_len[i] - 1; k > 0; k--) {
                    trails[i][k] = trails[i][k - 1];
                }
                trails[i][0] = matrix_chars[0]; // Brightest at head
            }
            if (y_pos[i] >= HEIGHT + trail_len[i] && rand() % 100 < 5) {
                y_pos[i] = 0;
                for (k = 0; k < trail_len[i]; k++) {
                    trails[i][k] = matrix_chars[k % matrix_chars_len];
                }
            }
        }

        // Render to buffer with color
        for (j = 0; j < HEIGHT; j++) {
            for (i = 0; i < WIDTH; i++) {
                int found = 0;
                for (k = 0; k < trail_len[i]; k++) {
                    if (j == y_pos[i] - k && j >= 0 && j < HEIGHT) {
                        // Cycle colors: 2 (green), 4 (red), 6 (yellow), 9 (blue)
                        int color = 2 + (i % 4); // Simple color cycle
                        set_color(color);
                        screen[j][i] = trails[i][k];
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    set_color(0); // Black background
                    screen[j][i] = ' ';
                }
            }
            screen[j][WIDTH] = '\0'; // Null-terminate each row
        }

        // Output buffer
        for (j = 0; j < HEIGHT; j++) {
            printf("%s\n", screen[j]);
        }

        sleep_ms(DELAY_MS);
    }

    // Reset color and clean up
    set_color(0x0A); // Back to green on black
    free(y_pos);
    free(trail_len);
    return 0;
}