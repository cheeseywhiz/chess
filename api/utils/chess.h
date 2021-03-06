/* the abstract chess game */
#pragma once
#include <vector>
#include <array>
#include <memory>

namespace Chess {
using std::vector;
using std::array;
using std::size_t;

enum class State {
    Ready,   // ready to select a piece
    Column1Selected,
    Row1Selected,
    Column2Selected,
    Promotion,
    Endgame,
};

enum class Piece { None, Pawn, Knight, Bishop, Rook, Queen, King };

enum class Player { None, White, Black };

struct Cell {
    Piece piece;
    Player player;
    bool has_moved;

    Cell() : piece(Piece::None), player(Player::None), has_moved(false) {}

    Cell(Piece piece_in, Player player_in, bool has_moved_in=false)
        : piece(piece_in), player(player_in), has_moved(has_moved_in) {}

    bool operator==(const Cell &cell) const {
        return piece == cell.piece \
            && player == cell.player \
            && has_moved == cell.has_moved;
    }

    bool operator!=(const Cell &cell) const {
        return !(*this == cell);
    }
};

static const int BOARD_WIDTH = 8, BOARD_HEIGHT = 8;
using BoardT = array<array<Cell, BOARD_WIDTH>, BOARD_HEIGHT>;
void init_board(BoardT&);

enum class EndgameState { None, Check, Checkmate, Stalemate };

struct ChessState {
    BoardT board;
    State state;
    Player player;
    EndgameState endgame_state;
    unsigned n_moves;
    vector<Piece> white_captures, black_captures;

    using Ptr = std::shared_ptr<ChessState>;

    ChessState() : state(State::Ready), player(Player::White),
                   endgame_state(EndgameState::None), n_moves(1) {
        init_board(board);
    }

    ChessState(
        const BoardT& board_in, State state_in, Player player_in, EndgameState endgame_state_in,
        unsigned n_moves_in, const vector<Piece>& white_captures_in,
        const vector<Piece>& black_captures_in
    )
        : board(board_in), state(state_in), player(player_in), endgame_state(endgame_state_in),
          n_moves(n_moves_in), white_captures(white_captures_in),
          black_captures(black_captures_in) {
    }
};

struct MoveResult {
    bool did_move;     // were we actually able to move the piece?
    bool can_promote;  // pawn has reached the end of the board?
    Piece capture;     // did the player capture an opponents piece?
};

MoveResult do_move(BoardT&, EndgameState, Player, size_t, size_t, size_t,
                   size_t);

struct CellReference {
    size_t row, col;

    CellReference(size_t row_in, size_t col_in) : row(row_in), col(col_in) {}

    bool operator==(const CellReference& other) const {
        return row == other.row && col == other.col;
    }
};

vector<CellReference> get_possible_moves(const BoardT&, size_t, size_t,
                                         bool do_check_check=true);

enum class Castles {
    None = 0,
    Queen = 1 << 0,
    King = 1 << 1,
};

Castles operator|=(Castles&, Castles);
bool operator&(Castles, Castles);

Castles get_possible_castles(const BoardT&, size_t, size_t, EndgameState);

EndgameState get_endgame_state(const BoardT&, Player);
} // namespace Chess
