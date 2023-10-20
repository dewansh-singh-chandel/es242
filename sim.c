#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#define BOARD_SIZE 6

// [ 0 , 1 ] , [ 1 , 0 ]  ->  0
// [ 0 , 2 ] , [ 2 , 0 ]  ->  1
// [ 0 , 3 ] , [ 3 , 0 ]  ->  2
// [ 0 , 4 ] , [ 4 , 0 ]  ->  3
// [ 0 , 5 ] , [ 5 , 0 ]  ->  4
// [ 1 , 2 ] , [ 2 , 1 ]  ->  5
// [ 1 , 3 ] , [ 3 , 1 ]  ->  6
// [ 1 , 4 ] , [ 4 , 1 ]  ->  7
// [ 1 , 5 ] , [ 5 , 1 ]  ->  8
// [ 2 , 3 ] , [ 3 , 2 ]  ->  9
// [ 2 , 4 ] , [ 4 , 2 ]  ->  10
// [ 2 , 5 ] , [ 5 , 2 ]  ->  11
// [ 3 , 4 ] , [ 4 , 3 ]  ->  12
// [ 3 , 5 ] , [ 5 , 3 ]  ->  13
// [ 4 , 5 ] , [ 5 , 4 ]  ->  14

void print_board(char *board) {
    for (int i = 0; i < (BOARD_SIZE * (BOARD_SIZE - 1) / 2); i++) {
        printf("%c ", board[i]);
    }
    printf("\n");
}

char* init_board() {
    char *board = (char *)malloc(sizeof(char) * (BOARD_SIZE * (BOARD_SIZE - 1) / 2));
    for (int i = 0; i < (BOARD_SIZE * (BOARD_SIZE - 1) / 2); i++) {
        board[i] = '.';
    }
    return board;
}

bool is_full(char *board) {
    for (int i = 0; i < (BOARD_SIZE * (BOARD_SIZE - 1) / 2); i++) {
        if (board[i] == '.') {
            return false;
        }
    }
    return true;
}

bool has_lost(char *board, char player) {
    int winning_combinations[][3] = {
        {0, 5, 1}, {0, 6, 2}, {0, 7, 3}, {0, 8, 4},
        {1, 9, 2}, {1, 10, 3}, {1, 11, 4},
        {2, 12, 3}, {2, 13, 4},
        {3, 14, 4},
        {5, 9, 6}, {5, 10, 7}, {5, 11, 8},
        {6, 12, 7}, {6, 13, 8},
        {7, 14, 8},
        {9, 12, 10}, {9, 13, 11},
        {10, 14, 11},
        {12, 14, 13}
    };

    for (int i = 0; i < sizeof(winning_combinations) / sizeof(winning_combinations[0]); i++) {
        int a = winning_combinations[i][0];
        int b = winning_combinations[i][1];
        int c = winning_combinations[i][2];
        if (board[a] == player && board[b] == player && board[c] == player) {
            return true;
        }
    }

    return false;
}

char other_player(char player) {
    return (player == 'B') ? 'R' : 'B';
}

typedef struct {
    int item;
    int score;
} Move;

Move best_move(char *board, char player) {
    Move best = {.score = INT_MIN};
    if (has_lost(board, player)) {
        best.score = -1;
        return best;
    }
    char opponent = other_player(player);
    if (has_lost(board, opponent)) {
        best.score = 1;
        return best;
    }
    if (is_full(board)) {
        best.score = 0;
        return best;
    }

    for (int i = 0; i < (BOARD_SIZE * (BOARD_SIZE - 1) / 2); i++) {
        if (board[i] == '.') {
            board[i] = player;
            int score = -best_move(board, opponent).score;
            board[i] = '.';
            if (score == 1) {
                return (Move){i, 1};
            }
            if (score > best.score) {
                best.item = i;
                best.score = score;
            }
        }
    }
    return best;
}

int main() {
    
    char *board = init_board();
    printf("Welcome to Game of Sim!\n");
    while (1) {
        print_board(board);
        int player_move;
        printf("Enter your move (0 - 14): ");
        scanf("%d", &player_move);

        if (board[player_move] != '.') {
            printf("Invalid move. Try again.\n");
            break;
        }

        board[player_move] = 'R';
        if (has_lost(board, 'R')) {
            print_board(board);
            printf("AI wins!\n");
            break;
        }

        if (is_full(board)) {
            print_board(board);
            printf("It's a tie!\n");
            break;
        }

        Move ai_move = best_move(board, 'B');
        board[ai_move.item] = 'B';

        if (has_lost(board, 'B')) {
            print_board(board);
            printf("You win!\n");
            break;
        }

        if (is_full(board)) {
            print_board(board);
            printf("It's a tie!\n");
            break;
        }
    }

    free(board);
    return 0;
}