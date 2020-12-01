// the abstract chess game
#include <unistd.h>  // size_t
#include "chess.hpp"

static void init_player_pieces(BoardT&, Player);

void
init_board(BoardT& board)
{
    init_player_pieces(board, Player::White);
    init_player_pieces(board, Player::Black);
}

static void
init_player_pieces(BoardT& board, Player player)
{
    size_t row = player == Player::White ? BOARD_HEIGHT - 2 : 1;
    for (size_t col = 0; col < BOARD_WIDTH; ++col)
        board[row][col] = Cell(Piece::Pawn, player);
    row = player == Player::White ? BOARD_HEIGHT - 1 : 0;
    board[row][0] = board[row][BOARD_WIDTH - 1] = Cell(Piece::Rook, player);
    board[row][1] = board[row][BOARD_WIDTH - 2] = Cell(Piece::Knight, player);
    board[row][2] = board[row][BOARD_WIDTH - 3] = Cell(Piece::Bishop, player);
    board[row][3] = Cell(Piece::Queen, player);
    board[row][4] = Cell(Piece::King, player);
}
