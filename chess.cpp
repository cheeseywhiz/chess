// the abstract chess game
#include <cassert>
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

static bool try_add_move(vector<CellReference>&, const BoardT&, Player, size_t,
                         size_t, bool guarantee_opponent=false);

/* generate the possible moves for the piece at the given cell */
vector<CellReference>
get_possible_moves(const BoardT& board, size_t row, size_t col)
{
    const Cell& cell = board[row][col];
    vector<CellReference> moves;
    assert(cell.player != Player::None);

    if (cell.piece == Piece::Pawn) {
        if (cell.player == Player::White) {  // moves "up" (lower row)
            if (try_add_move(moves, board, cell.player, row - 1, col)) {
                if (!cell.has_moved)
                    try_add_move(moves, board, cell.player, row - 2, col);
            }

            try_add_move(moves, board, cell.player, row - 1, col - 1, true);
            try_add_move(moves, board, cell.player, row - 1, col + 1, true);
        } else {  // black moves "down" (higher row)
            if (try_add_move(moves, board, cell.player, row + 1, col)) {
                if (!cell.has_moved)
                    try_add_move(moves, board, cell.player, row + 2, col);
            }

            try_add_move(moves, board, cell.player, row + 1, col - 1, true);
            try_add_move(moves, board, cell.player, row + 1, col + 1, true);
        }
    }

    // horizontal moves
    if (cell.piece == Piece::Rook || cell.piece == Piece::Queen) {
        size_t i = row;
        while (try_add_move(moves, board, cell.player, --i, col))
            ;

        i = row;
        while (try_add_move(moves, board, cell.player, ++i, col))
            ;

        size_t j = col;
        while (try_add_move(moves, board, cell.player, row, --j))
            ;

        j = col;
        while (try_add_move(moves, board, cell.player, row, ++j))
            ;
    }

    // diagonal moves
    if (cell.piece == Piece::Bishop || cell.piece == Piece::Queen) {
        size_t i, j;

        i = row;
        j = col;
        while (try_add_move(moves, board, cell.player, --i, --j))
            ;

        i = row;
        j = col;
        while (try_add_move(moves, board, cell.player, --i, ++j))
            ;

        i = row;
        j = col;
        while (try_add_move(moves, board, cell.player, ++i, --j))
            ;

        i = row;
        j = col;
        while (try_add_move(moves, board, cell.player, ++i, ++j))
            ;
    }

    if (cell.piece == Piece::Knight) {
        try_add_move(moves, board, cell.player, row - 2, col - 1);
        try_add_move(moves, board, cell.player, row - 2, col + 1);
        try_add_move(moves, board, cell.player, row - 1, col - 2);
        try_add_move(moves, board, cell.player, row - 1, col + 2);
        try_add_move(moves, board, cell.player, row + 2, col - 1);
        try_add_move(moves, board, cell.player, row + 2, col + 1);
        try_add_move(moves, board, cell.player, row + 1, col - 2);
        try_add_move(moves, board, cell.player, row + 1, col + 2);
    } else if (cell.piece == Piece::King) {
        try_add_move(moves, board, cell.player, row - 1, col - 1);
        try_add_move(moves, board, cell.player, row - 1, col);
        try_add_move(moves, board, cell.player, row - 1, col + 1);
        try_add_move(moves, board, cell.player, row, col - 1);
        try_add_move(moves, board, cell.player, row, col + 1);
        try_add_move(moves, board, cell.player, row + 1, col - 1);
        try_add_move(moves, board, cell.player, row + 1, col);
        try_add_move(moves, board, cell.player, row + 1, col + 1);
    }

    return moves;
}

/* return if we can keep trying to add more moves
 * guarantee_opponent: don't move if the chosen cell is not the opponent */
static bool
try_add_move(vector<CellReference>& moves, const BoardT& board, Player player,
             size_t row, size_t col, bool guarantee_opponent)
{
    if (row >= BOARD_HEIGHT || col >= BOARD_WIDTH)
        return false;
    const Cell& cell = board[row][col];
    if (cell.player == player)
        return false;
    if (guarantee_opponent && cell.player == Player::None)
        return false;
    moves.emplace_back(row, col);
    return cell.player == Player::None;
}
