/* the abstract chess game */
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include "chess.hpp"
using std::find;
using std::runtime_error;
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
    if (player == Player::None)
        return;
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
do_move(
    BoardT& board, EndgameState endgame_state, Player player, size_t row1,
    size_t column1, size_t row2, size_t column2
) {
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
    Castles castles = get_possible_castles(board, row1, column1, endgame_state);

    if (castles & Castles::Queen
            && row2 == row1
            && column2 == 2) {
        board[row1][0] = Cell();
        board[row1][2] = Cell(Piece::King, player, true);
        board[row1][3] = Cell(Piece::Rook, player, true);
        board[row1][4] = Cell();
        result.did_move = true;
        return result;
    }

    if (castles & Castles::King
            && row2 == row1
            && column2 == BOARD_WIDTH - 2) {
        board[row1][BOARD_WIDTH - 1] = Cell();
        board[row1][BOARD_WIDTH - 2] = Cell(Piece::King, player, true);
        board[row1][BOARD_WIDTH - 3] = Cell(Piece::Rook, player, true);
        board[row1][BOARD_WIDTH - 4] = Cell();
        result.did_move = true;
        return result;
    }

    const vector<CellReference> moves = \
        get_possible_moves(board, row1, column1);
    const auto& it = \
        find(moves.begin(), moves.end(), CellReference(row2, column2));
    // can't move to non-possible move
    if (it == moves.end())
        return result;
    result.did_move = true;
    if (cell2.piece != Piece::None)
        result.capture = cell2.piece;
    cell2 = std::move(cell1);
    cell2.has_moved = true;
    cell1 = Cell();
    return result;
}

enum MoveType { Normal, PawnStright, PawnDiagonal };

static bool try_add_move(
    vector<CellReference>*, const BoardT&, CellReference, CellReference,
    bool, MoveType move_type=MoveType::Normal);

/* generate the possible moves for the piece at the given cell */
vector<CellReference>
get_possible_moves(const BoardT& board, size_t row, size_t col,
                   bool do_check_check)
{
    const Cell& cell = board[row][col];
    CellReference cell_ref = { row, col };
    vector<CellReference> moves;
    assert(cell.player != Player::None);

    if (cell.piece == Piece::Pawn) {
        if (cell.player == Player::White) {  // moves "up" (lower row)
            if (try_add_move(&moves, board, cell_ref, { row - 1, col },
                             do_check_check, MoveType::PawnStright)) {
                if (!cell.has_moved)
                    try_add_move(&moves, board, cell_ref, { row - 2, col },
                                 do_check_check, MoveType::PawnStright);
            }

            try_add_move(&moves, board, cell_ref, { row - 1, col - 1 },
                         do_check_check, MoveType::PawnDiagonal);
            try_add_move(&moves, board, cell_ref, { row - 1, col + 1 },
                         do_check_check, MoveType::PawnDiagonal);
        } else {  // black moves "down" (higher row)
            if (try_add_move(&moves, board, cell_ref, { row + 1, col },
                             do_check_check, MoveType::PawnStright)) {
                if (!cell.has_moved)
                    try_add_move(&moves, board, cell_ref, { row + 2, col },
                                 do_check_check, MoveType::PawnStright);
            }

            try_add_move(&moves, board, cell_ref, { row + 1, col - 1 },
                         do_check_check, MoveType::PawnDiagonal);
            try_add_move(&moves, board, cell_ref, { row + 1, col + 1 },
                         do_check_check, MoveType::PawnDiagonal);
        }
    }

    // horizontal moves
    if (cell.piece == Piece::Rook || cell.piece == Piece::Queen) {
        size_t i = row;
        while (try_add_move(&moves, board, cell_ref, { --i, col },
                            do_check_check))
            ;

        i = row;
        while (try_add_move(&moves, board, cell_ref, { ++i, col },
                            do_check_check))
            ;

        size_t j = col;
        while (try_add_move(&moves, board, cell_ref, { row, --j },
                            do_check_check))
            ;

        j = col;
        while (try_add_move(&moves, board, cell_ref, { row, ++j },
                            do_check_check))
            ;
    }

    // diagonal moves
    if (cell.piece == Piece::Bishop || cell.piece == Piece::Queen) {
        size_t i, j;

        i = row;
        j = col;
        while (try_add_move(&moves, board, cell_ref, { --i, --j },
                            do_check_check))
            ;

        i = row;
        j = col;
        while (try_add_move(&moves, board, cell_ref, { --i, ++j },
                            do_check_check))
            ;

        i = row;
        j = col;
        while (try_add_move(&moves, board, cell_ref, { ++i, --j },
                            do_check_check))
            ;

        i = row;
        j = col;
        while (try_add_move(&moves, board, cell_ref, { ++i, ++j },
                            do_check_check))
            ;
    }

    if (cell.piece == Piece::Knight) {
        try_add_move(&moves, board, cell_ref, { row - 2, col - 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row - 2, col + 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row - 1, col - 2 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row - 1, col + 2 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 2, col - 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 2, col + 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 1, col - 2 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 1, col + 2 },
                     do_check_check);
    } else if (cell.piece == Piece::King) {
        try_add_move(&moves, board, cell_ref, { row - 1, col - 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row - 1, col },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row - 1, col + 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row, col - 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row, col + 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 1, col - 1 },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 1, col },
                     do_check_check);
        try_add_move(&moves, board, cell_ref, { row + 1, col + 1 },
                     do_check_check);
    }

    return moves;
}

static bool player_is_in_check(const BoardT&, Player);

/* return if we can move cell1 to cell2
 * move_type handles special cases for pawn moves and castles */
static bool
try_add_move(
    vector<CellReference>* moves, const BoardT& board, CellReference cell_ref_1,
    CellReference cell_ref_2, bool do_check_check, MoveType move_type)
{
    if (cell_ref_2.row >= BOARD_HEIGHT || cell_ref_2.col >= BOARD_WIDTH)
        return false;
    const Cell& cell1 = board[cell_ref_1.row][cell_ref_1.col];
    const Cell& cell2 = board[cell_ref_2.row][cell_ref_2.col];
    // cannot move into our own piece
    if (cell2.player == cell1.player)
        return false;
    // if straight is not empty, then cannot move
    if (move_type == MoveType::PawnStright && cell2.piece != Piece::None)
        return false;
    // if the diagonal is not the opponent, then cannot move
    if (move_type == MoveType::PawnDiagonal && cell2.player == Player::None)
        return false;

    if (do_check_check) {
        // try to move the piece and see if we'll be in check
        BoardT test_board = board;
        Cell& test_cell_1 = test_board[cell_ref_1.row][cell_ref_1.col];
        Cell& test_cell_2 = test_board[cell_ref_2.row][cell_ref_2.col];
        test_cell_2 = std::move(test_cell_1);
        test_cell_2.has_moved = true;
        test_cell_1 = Cell();
        if (player_is_in_check(test_board, cell1.player))
            // do not add the move, but keep trying more moves
            return true;
    }

    if (moves)
        moves->push_back(cell_ref_2);
    return cell2.player == Player::None;
}

Castles
get_possible_castles(const BoardT& board, size_t row, size_t column,
                     EndgameState endgame_state)
{
    const Cell& cell = board[row][column];
    Castles castles = Castles::None;
    if (cell.piece != Piece::King || endgame_state == EndgameState::Check)
        return castles;
    const CellReference king_ref = { row, 4 };
    if (board[row][4] != Cell(Piece::King, cell.player))
        return castles;

    if (board[row][0] == Cell(Piece::Rook, cell.player)
            && board[row][1] == Cell()
            && board[row][2] == Cell()
            && board[row][3] == Cell()
            && try_add_move(nullptr, board, king_ref, { row, 3 }, true)
            && try_add_move(nullptr, board, king_ref, { row, 2 }, true)
    ) {
        castles |= Castles::Queen;
    }

    if (board[row][BOARD_WIDTH - 1] == Cell(Piece::Rook, cell.player)
            && board[row][BOARD_WIDTH - 2] == Cell()
            && board[row][BOARD_WIDTH - 3] == Cell()
            && try_add_move(nullptr, board, king_ref, { row, BOARD_WIDTH - 3 },
                            true)
            && try_add_move(nullptr, board, king_ref, { row, BOARD_WIDTH - 2 },
                            true)
    ) {
        castles |= Castles::King;
    }

    return castles;
}

Castles
operator|=(Castles& castles1, Castles castles2)
{
    return (castles1 = static_cast<Castles>(
        static_cast<int>(castles1) | static_cast<int>(castles2)
    ));
}

bool
operator&(Castles castles1, Castles castles2)
{
    return static_cast<int>(castles1) & static_cast<int>(castles2);
}

static bool player_has_no_moves(const BoardT&, Player, bool);

/* get the state of the game given that the chosen player takes the next move */
EndgameState
get_endgame_state(const BoardT& board, Player player)
{
    bool is_in_check = player_is_in_check(board, player);
    if (player_has_no_moves(board, player, is_in_check))
        return is_in_check ? EndgameState::Checkmate : EndgameState::Stalemate;
    return is_in_check ? EndgameState::Check : EndgameState::None;
}

static CellReference find_king(const BoardT&, Player);

/* is the player in check? */
static bool
player_is_in_check(const BoardT& board, Player player)
{
    CellReference king_ref = find_king(board, player);
    Player opponent = player == Player::White ? Player::Black : Player::White;

    for (size_t row = 0; row < BOARD_HEIGHT; ++row) {
        for (size_t col = 0; col < BOARD_WIDTH; ++col) {
            const Cell& cell = board[row][col];
            if (cell.player != opponent)
                continue;
            const vector<CellReference> moves = \
                get_possible_moves(board, row, col, false);
            const auto& it = find(moves.begin(), moves.end(), king_ref);
            // king in possible moves?
            if (it != moves.end())
                return true;
        }
    }

    // XXX: pretty sure we don't have to consider castling here, but I'm not
    // sure
    return false;
}

/* return the cell for the player's king */
static CellReference
find_king(const BoardT& board, Player player)
{
    for (size_t row = 0; row < BOARD_HEIGHT; ++row) {
        for (size_t col = 0; col < BOARD_WIDTH; ++col) {
            const Cell& cell = board[row][col];
            if (cell.player == player && cell.piece == Piece::King)
                return { row, col };
        }
    }

    throw runtime_error("player has no king on the board");
}

/* this can be used to determine stalemate or checkmate */
static bool
player_has_no_moves(const BoardT& board, Player player, bool is_in_check)
{
    for (size_t row = 0; row < BOARD_HEIGHT; ++row) {
        for (size_t col = 0; col < BOARD_WIDTH; ++col) {
            if (board[row][col].player == player
                    && !get_possible_moves(board, row, col).empty())
                return false;
        }
    }

    CellReference king_ref = find_king(board, player);
    return get_possible_castles(
        board, king_ref.row, king_ref.col,
        is_in_check ? EndgameState::Check : EndgameState::None
    ) == Castles::None;
}
