// the abstract chess game
#pragma once
#include <vector>
#include <array>
using std::vector;
using std::array;
using std::size_t;

enum class State {
    Ready,   /* ready to select a piece */
    Column1Selected,
    Row1Selected,
    Column2Selected,
    // Row2Selected,
};

enum class Piece { None, Pawn, Knight, Bishop, Rook, Queen, King };

enum class Player { None, White, Black };

struct Cell {
    Piece piece;
    Player player;
    bool has_moved;

    Cell() : piece(Piece::None), player(Player::None), has_moved(false) {}

    Cell(Piece piece_in, Player player_in)
        : piece(piece_in), player(player_in), has_moved(false) {}
};

static const int BOARD_WIDTH = 8, BOARD_HEIGHT = 8;
using BoardT = array<array<Cell, BOARD_WIDTH>, BOARD_HEIGHT>;
void init_board(BoardT&);

struct MoveResult {
    bool did_move;  /* were we actually able to move the piece? */
    Piece capture;  /* did the player capture an opponents piece? */
};

MoveResult do_move(BoardT&, Player, size_t, size_t, size_t, size_t);

struct CellReference {
    size_t row, col;

    CellReference(size_t row_in, size_t col_in) : row(row_in), col(col_in) {}

    bool operator==(const CellReference& other) const {
        return row == other.row && col == other.col;
    }
};

vector<CellReference> get_possible_moves(const BoardT&, size_t, size_t,
                                         bool do_check_check=true);
