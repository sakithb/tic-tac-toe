#include <raylib.h>
#include <stdbool.h>
#include <string.h>

bool has_won(char board[3][3], char player) {
    return (player == board[0][0] && player == board[0][1] && player == board[0][2]) ||
           (player == board[1][0] && player == board[1][1] && player == board[1][2]) ||
           (player == board[2][0] && player == board[2][1] && player == board[2][2]) ||
           (player == board[0][0] && player == board[1][0] && player == board[2][0]) ||
           (player == board[0][1] && player == board[1][1] && player == board[2][1]) ||
           (player == board[0][2] && player == board[1][2] && player == board[2][2]) ||
           (player == board[0][0] && player == board[1][1] && player == board[2][2]) ||
           (player == board[0][2] && player == board[1][1] && player == board[2][0]);
}

bool has_drawed(char board[3][3]) {
    int empty_count = 0;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == 0)
                empty_count++;
        }
    }

    return empty_count == 0;
}

typedef struct {
    int row;
    int col;
    int score;
} MinimaxResult;

MinimaxResult minimax(char board[3][3], int depth, bool is_maximizer) {
    MinimaxResult result = {0, 0, 0};

    if (has_won(board, 'O')) {
        result.score = 10;
    } else if (has_won(board, 'X')) {
        result.score = -10;
    } else if (!has_drawed(board) && depth > 0) {
        if (is_maximizer) {
            result.score = -11;

            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (board[row][col] != 0)
                        continue;

                    board[row][col] = 'O';

                    MinimaxResult r = minimax(board, depth - 1, false);
                    if (r.score > result.score) {
                        result.row = row;
                        result.col = col;
                        result.score = r.score;
                    }

                    board[row][col] = 0;
                }
            }
        } else {
            result.score = 11;

            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (board[row][col] != 0)
                        continue;

                    board[row][col] = 'X';

                    MinimaxResult r = minimax(board, depth - 1, true);
                    if (r.score < result.score) {
                        result.row = row;
                        result.col = col;
                        result.score = r.score;
                    }

                    board[row][col] = 0;
                }
            }
        }
    }

    return result;
}

void display_banner(const char *text) {
    int width = MeasureText(text, 50);
    DrawText(text, 240 - (width / 2), 440 - 25, 50, WHITE);
}

int main() {
    InitWindow(480, 480, "Tic Tac Toe");

    char board[3][3] = {0};
    bool is_user_turn = true;
    bool game_over = false;

    Vector2 xsize = MeasureTextEx(GetFontDefault(), "X", 80, 0);
    Vector2 osize = MeasureTextEx(GetFontDefault(), "O", 80, 0);
    int xxoff = (100 - xsize.x) / 2;
    int xyoff = (100 - xsize.y) / 2;
    int oxoff = (100 - osize.x) / 2;
    int oyoff = (100 - osize.y) / 2;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (game_over) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                game_over = false;
                is_user_turn = true;
                memset(board, 0, sizeof(char) * 9);

                ClearBackground(BLACK);
            }
        } else {
            if (is_user_turn) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    Vector2 pos = GetMousePosition();
                    if (pos.x > 90 && pos.x < 390 && pos.y > 90 && pos.y < 390) {
                        int r = (pos.x - 90) / 100;
                        int c = (pos.y - 90) / 100;
                        if (board[r][c] == 0) {
                            board[r][c] = 'X';
                            is_user_turn = false;
                        }
                    }
                }
            } else {
                MinimaxResult move = minimax(board, 3, true);
                board[move.row][move.col] = 'O';
                is_user_turn = true;
            }
        }

        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                DrawRectangleLines(90 + (r * 100), 90 + (c * 100), 100, 100, WHITE);
                if (board[r][c] == 'X') {
                    DrawText("X", 90 + (r * 100) + xxoff, 90 + (c * 100) + xyoff, 80, WHITE);
                } else if (board[r][c] == 'O') {
                    DrawText("O", 90 + (r * 100) + oxoff, 90 + (c * 100) + oyoff, 80, WHITE);
                }
            }
        }

        if (has_won(board, 'X')) {
            display_banner("You won!");
            game_over = true;
        } else if (has_won(board, 'O')) {
            display_banner("You lost!");
            game_over = true;
        } else if (has_drawed(board)) {
            display_banner("Draw!");
            game_over = true;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
