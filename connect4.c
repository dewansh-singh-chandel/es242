#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define ROWS 6
#define COLS 7
#define MAX_DEPTH 6

char board[ROWS][COLS];

void init_board()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void print_board()
{
    printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
    }
    printf("|---|---|---|---|---|---|---|\n");
    printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
}

bool is_valid_move(int col)
{
    return col >= 0 && col < COLS && board[0][col] == ' ';
}

int drop_disc(int col, char player)
{
    for (int i = ROWS - 1; i >= 0; i--)
    {
        if (board[i][col] == ' ')
        {
            board[i][col] = player;
            return i;
        }
    }
    return -1; // Column is full
}

bool check_win(int row, int col, char player)
{
    // Check horizontally
    int count = 0;
    for (int i = col - 3; i <= col + 3; i++)
    {
        if (i >= 0 && i < COLS && board[row][i] == player)
        {
            count++;
            if (count == 4)
                return true;
        }
        else
        {
            count = 0;
        }
    }

    // Check vertically
    count = 0;
    for (int i = row - 3; i <= row + 3; i++)
    {
        if (i >= 0 && i < ROWS && board[i][col] == player)
        {
            count++;
            if (count == 4)
                return true;
        }
        else
        {
            count = 0;
        }
    }

    // Check diagonally (top-left to bottom-right)
    count = 0;
    for (int i = -3; i <= 3; i++)
    {
        int r = row + i;
        int c = col + i;
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == player)
        {
            count++;
            if (count == 4)
                return true;
        }
        else
        {
            count = 0;
        }
    }

    // Check diagonally (bottom-left to top-right)
    count = 0;
    for (int i = -3; i <= 3; i++)
    {
        int r = row - i;
        int c = col + i;
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == player)
        {
            count++;
            if (count == 4)
                return true;
        }
        else
        {
            count = 0;
        }
    }

    return false;
}

bool is_board_full()
{
    for (int i = 0; i < COLS; i++)
    {
        if (board[0][i] == ' ')
        {
            return false;
        }
    }
    return true;
}

int evaluate_board(char player)
{
    int score = 0;

    // Check horizontally, vertically, and diagonally for both players
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (board[row][col] == player)
            {
                // Horizontal
                int horizontal = 0;
                for (int i = -3; i <= 3; i++)
                {
                    if (col + i >= 0 && col + i + 3 < COLS)
                    {
                        int count = 0;
                        for (int j = 0; j < 4; j++)
                        {
                            if (board[row][col + i + j] == player)
                            {
                                count++;
                            }
                            else if (board[row][col + i + j] != ' ')
                            {
                                count = 0; // Blocked by opponent
                                break;
                            }
                        }
                        horizontal += count;
                    }
                }

                // Vertical
                int vertical = 0;
                for (int i = -3; i <= 3; i++)
                {
                    if (row + i >= 0 && row + i + 3 < ROWS)
                    {
                        int count = 0;
                        for (int j = 0; j < 4; j++)
                        {
                            if (board[row + i + j][col] == player)
                            {
                                count++;
                            }
                            else if (board[row + i + j][col] != ' ')
                            {
                                count = 0; // Blocked by opponent
                                break;
                            }
                        }
                        vertical += count;
                    }
                }

                // Diagonal (downward)
                int diagonal_down = 0;
                for (int i = -3; i <= 3; i++)
                {
                    if (row + i >= 0 && row + i + 3 < ROWS && col + i >= 0 && col + i + 3 < COLS)
                    {
                        int count = 0;
                        for (int j = 0; j < 4; j++)
                        {
                            if (board[row + i + j][col + i + j] == player)
                            {
                                count++;
                            }
                            else if (board[row + i + j][col + i + j] != ' ')
                            {
                                count = 0; // Blocked by opponent
                                break;
                            }
                        }
                        diagonal_down += count;
                    }
                }

                // Diagonal (upward)
                int diagonal_up = 0;
                for (int i = -3; i <= 3; i++)
                {
                    if (row - i >= 0 && row - i - 3 < ROWS && col + i >= 0 && col + i + 3 < COLS)
                    {
                        int count = 0;
                        for (int j = 0; j < 4; j++)
                        {
                            if (board[row - i - j][col + i + j] == player)
                            {
                                count++;
                            }
                            else if (board[row - i - j][col + i + j] != ' ')
                            {
                                count = 0; // Blocked by opponent
                                break;
                            }
                        }
                        diagonal_up += count;
                    }
                }

                // Evaluate the positions
                score += horizontal + vertical + diagonal_down + diagonal_up;
            }
        }
    }

    return score;
}

int minimax(int depth, int alpha, int beta, bool is_maximizing, char player)
{
    if (depth == 0 || check_win(0, 0, 'X') || check_win(0, 0, 'O') || is_board_full())
    {
        return evaluate_board(player);
    }

    char opponent = (player == 'X') ? 'O' : 'X';

    if (is_maximizing)
    {
        int max_eval = INT_MIN;
        for (int col = 0; col < COLS; col++)
        {
            if (is_valid_move(col))
            {
                int row = drop_disc(col, player);
                int eval = minimax(depth - 1, alpha, beta, false, player);
                max_eval = (eval > max_eval) ? eval : max_eval;
                alpha = (alpha > eval) ? alpha : eval;
                board[row][col] = ' ';
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return max_eval;
    }
    else
    {
        int min_eval = INT_MAX;
        for (int col = 0; col < COLS; col++)
        {
            if (is_valid_move(col))
            {
                int row = drop_disc(col, opponent);
                int eval = minimax(depth - 1, alpha, beta, true, player);
                min_eval = (eval < min_eval) ? eval : min_eval;
                beta = (beta < eval) ? beta : eval;
                board[row][col] = ' ';
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return min_eval;
    }
}

int best_move(char player)
{
    int best_col = -1;
    int best_eval = INT_MIN;

    for (int col = 0; col < COLS; col++)
    {
        if (is_valid_move(col))
        {
            int row = drop_disc(col, player);
            int eval = minimax(MAX_DEPTH, INT_MIN, INT_MAX, false, player);
            board[row][col] = ' ';
            if (eval > best_eval)
            {
                best_eval = eval;
                best_col = col;
            }
        }
    }

    return best_col;
}

int main()
{
    init_board();
    char player = 'X';
    int moves = 0;

    while (true)
    {
        print_board();

        if (player == 'X')
        {
            printf("Player X, enter your move (1-7): ");
            int col;
            scanf("%d", &col);
            col--;

            if (is_valid_move(col))
            {
                int row = drop_disc(col, player);
                if (check_win(row, col, player))
                {
                    print_board();
                    printf("Player X wins!\n");
                    break;
                }
                else if (is_board_full())
                {
                    print_board();
                    printf("It's a draw!\n");
                    break;
                }
                player = 'O';
            }
            else
            {
                printf("Invalid move. Try again.\n");
            }
        }
        else
        { // Computer's turn
            printf("Computer's move...\n");
            int col;

            col = best_move(player);

            int row = drop_disc(col, player);
            if (check_win(row, col, player))
            {
                print_board();
                printf("Computer (Player O) wins!\n");
                break;
            }
            else if (is_board_full())
            {
                print_board();
                printf("It's a draw!\n");
                break;
            }
            player = 'X';
        }
    }

    return 0;
}