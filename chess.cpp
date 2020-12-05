// the abstract chess game
#include <algorithm>
#include <vector>
#include "chess.hpp"
using std::find;
using std::vector;

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
    board[row][BOARD_WIDTH - 4] = Cell(Piece::King, player);
}

MoveResult
do_move(BoardT& board, Player player, size_t row1, size_t column1,
        size_t row2, size_t column2)
{
    MoveResult result = { false, Piece::None };
    // can't move invalid selection
    if (row1>= BOARD_HEIGHT || column1 >= BOARD_WIDTH
            || row2 >= BOARD_HEIGHT || column2 >= BOARD_WIDTH)
        return result;
    Cell &cell1 = board[row1][column1];
    // verify the right color was selected
    if (cell1.player != player)
        return result;
    // can't move non-piece
    if (cell1.piece == Piece::None || cell1.player == Player::None)
        return result;
    Cell& cell2 = board[row2][column2];
    // can't move piece into our own piece
    if (cell2.player == player)
        return result;
    const vector<CellReference> moves = \
        get_possible_moves(board, row1, column1);
    const auto it = \
        find(moves.begin(), moves.end(), CellReference(row2, column2));
    // can't move to non-possible move
    if (it == moves.end())
        return result;
    result.did_move = true;
    if (cell2.piece != Piece::None)
        result.capture = cell2.piece;
    cell2 = std::move(cell1);
    cell2.has_moved = true;
    cell1 = Cell(Piece::None, Player::None);
    return result;
}

vector<CellReference>
get_possible_moves(const BoardT& board, size_t row, size_t col)
{
    const Cell& cell = board[row][col];
    vector<CellReference> moves;

    if (cell.piece == Piece::Pawn) {
        if (cell.player == Player::White) {  // moves "up" (lower row)
            if (row) {
                // space in front is free?
                if (board[row - 1][col].piece == Piece::None) {
                    moves.emplace_back(row - 1, col);
                    // has not moved and two spaces in front is free?
                    if (!cell.has_moved
                            && board[row - 2][col].piece == Piece::None)
                        moves.emplace_back(row - 2, col);
                }

                // can take forward left?
                if (col && board[row - 1][col - 1].player == Player::Black)
                    moves.emplace_back(row - 1, col - 1);
                // can take forward right?
                if (col != BOARD_WIDTH - 1
                        && board[row - 1][col + 1].player == Player::Black)
                    moves.emplace_back(row - 1, col + 1);
            }  // else if at the top row, no possible moves
        } else {  // black moves "down" (higher row)
            if (row != BOARD_HEIGHT - 1) {
                // space in front is free?
                if (board[row + 1][col].piece == Piece::None) {
                    moves.emplace_back(row + 1, col);
                    // has not moved and two spaces in front is free?
                    if (!cell.has_moved
                            && board[row + 2][col].piece == Piece::None)
                        moves.emplace_back(row + 2, col);
                }

                // can take forward left?
                if (col != BOARD_WIDTH - 1
                        && board[row + 1][col + 1].player == Player::White)
                    moves.emplace_back(row + 1, col + 1);
                // can take forward right?
                if (col && board[row + 1][col - 1].player == Player::White)
                    moves.emplace_back(row + 1, col - 1);
            }  // else if at the top row, no possible moves
        }
    }

    return moves;
}
