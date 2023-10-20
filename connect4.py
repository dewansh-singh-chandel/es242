ROWS = 4
COLS = 5

def init_board():
    return [[' ' for _ in range(COLS)] for _ in range(ROWS)]

def print_board(board):
    print()
    for row in board:
        print("|", end=" ")
        for cell in row:
            print(cell, "|", end=" ")
        print()
    print("|---|---|---|---|---|")
    print("| 1 | 2 | 3 | 4 | 5 |")

def is_valid_move(board, col):
    return col >= 0 and col < COLS and board[0][col] == ' '

def drop_disc(board, col, player):
    for row in range(ROWS - 1, -1, -1):
        if board[row][col] == ' ':
            board[row][col] = player
            return row
    return -1

def check_win(board, player):
    def check_direction(dx, dy):
        for r in range(ROWS):
            for c in range(COLS):
                if 0 <= r + 3 * dy < ROWS and 0 <= c + 3 * dx < COLS:
                    count = 0
                    for i in range(4):
                        if board[r + i * dy][c + i * dx] == player:
                            count += 1
                    if count == 4:
                        return True
        return False

    directions = [(1, 0), (0, 1), (1, 1), (1, -1)]
    for dx, dy in directions:
        if check_direction(dx, dy):
            return True
    return False

def is_board_full(board):
    for col in board[0]:
        if col == ' ':
            return False
    return True

def other_player(player):
    return 'O' if player == 'X' else 'X'

def best_move(board, player, memo):
    if check_win(board, player):
        return {'score': 1}
    elif check_win(board, other_player(player)):
        return {'score': -1}
    elif is_board_full(board):
        return {'score': 0}

    best_moove = None
    best_score = float('-inf')

    for col in range(COLS):
        if is_valid_move(board, col):
            row = drop_disc(board, col, player)
            key = str(board)
            if key in memo:
                score = memo[key]
            else:
                score = -best_move(board, other_player(player), memo)['score']
                memo[key] = score
            board[row][col] = ' '

            if score == 1:
                return {'col': col, 'score': score}

            if score > best_score:
                best_score = score
                best_moove = {'col': col, 'score': score}

    return best_moove

def main():
    board = init_board()
    player = 'X'
    memo = {}
    while True:
        print_board(board)

        if player == 'X':
            print("Player X, enter your move (1-5): ")
            col = int(input()) - 1

            if is_valid_move(board, col):
                row = drop_disc(board, col, player)
                board[row][col] = 'X'
                if check_win(board, player):
                    print_board(board)
                    print("Player X wins!")
                    break
                elif is_board_full(board):
                    print_board(board)
                    print("It's a draw!")
                    break
                player = 'O'
            else:
                print("Invalid move. Try again.")
        else:
            print("Computer's move...")
            col = best_move(board, player, memo)['col']
            row = drop_disc(board, col, player)
            board[row][col] = 'O'
            if check_win(board, player):
                print_board(board)
                print("Computer (Player O) wins!")
                break
            elif is_board_full(board):
                print_board(board)
                print("It's a draw!")
                break
            player = 'X'

if __name__ == "__main__":
    main()
