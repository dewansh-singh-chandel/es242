#include <stdio.h>
#include <assert.h>

#include "test.h"

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};


/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */


player_t other_player(player_t player){
    switch (player)
    {
    case 'R':
        return 'B';
        
    
    case 'B':
        return 'R';
        
    }
}
int has_won(board_t board, player_t player)
{
    int winning_combinations[20][3] = {
        {0, 5, 1}, {0, 6, 2}, {0, 7, 3}, {0, 8, 4}, {1, 9, 2}, {1, 10, 3}, {1, 11, 4},
        {2, 12, 3}, {2, 13, 4}, {3, 14, 4}, {5, 9, 6}, {5, 10, 7}, {5, 11, 8},
        {6, 12, 7}, {6, 13, 8}, {7, 14, 8}, {9, 12, 10}, {9, 13, 11}, {10, 14, 11}, {12, 14, 13}
    };

    for (int i = 0; i < 20; i++) {
        int a = winning_combinations[i][0];
        int b = winning_combinations[i][1];
        int c = winning_combinations[i][2];
        if (board[a] == player && board[b] == player && board[c] == player) {
            return 1;
        }
    }

    return 0;
 }

int is_full(board_t board)   // Actually no need for is full functions since there will always be a winner
{
    int flag = 1;
    for (int i = 0; i < 15; i++)
    {
        if( board[i]== RED || board[i] ==BLUE){

            continue;

        } 
        else{
            flag = 0;
            return 0;
            break;
        }
    }

    return flag;

    
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

move_t best_move(board_t board, player_t player)
{
    move_t  b_move;
    b_move.score = -1;

    if (has_won(board, player)) {
        b_move.score = 1;
        return b_move;
    }
    if (has_won(board, other_player(player))) {
        b_move.score = -1;
        return b_move;
    }
    if (is_full(board)) {
        b_move.score = 0;
        return b_move;
    }

    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            board[i] = player;
            if(has_won(board,player)){
                board[i] = NO;
                move_t move ;
                move.line = i;
                move.score = 1;
                return move;
            }
            board[i]= NO;
        }
    }

    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            board[i] = player;
            move_t move = best_move(board,other_player(player));
            move.score = -move.score;
            board[i] = NO;

            if (move.score == 1) {
                move.line = i;
                return move;
            }

            if (move.score > b_move.score) {
                b_move.score = move.score;
                b_move.line = i;
            }
        }
    }

    return b_move;
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case RED: printf("R  "); break;
        case BLUE: printf("B  "); break;
        case NO : printf("0 "); break;
        }
    }
    printf("\n");
}

int position(player_t player){

    switch (player)
    {
    case 'R':
        return RED;
        break;
    
    default:
        return BLUE;
        break;
    }
}



int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    printf("Select your color R or B\n");
    player_t user ;
    scanf("%c",&user);
    board_t board = { NO };
    while (1)
    {
        /* code */
    
    
    print_board(board);
    printf("Select you index from 0 to 14 %c player \n", user );

    int pos;
    scanf("%d",&pos);
      
    board[pos] = position(user);
    print_board(board);
    
    move_t move = best_move(board,other_player(user));
    
    board[move.line] = position(other_player(user));
    if(has_won(board,user)){
        printf("You won");
        break;
    }
    if(has_won(board,other_player(user))){
        printf("Computer won");
        break;
    }

    }
    return 0;
}
