#pragma once
#include <array>
using std::array;

static const short WhiteOnWhite = 1,
                   WhiteOnBlack = 2,
                   BlackOnWhite = 3,
                   BlackOnBlack = 4;
static const short WhiteBlank = BlackOnWhite,
                   BlackBlank = WhiteOnBlack;

enum class Piece { None, Pawn, Knight, Bishop, Rook, Queen, King };

static const unsigned piece_char[] = {
    ' ', /* None */
    'p', /* Pawn */
    'N', /* Knight */
    'B', /* Bishop */
    'R', /* Rook */
    'Q', /* Queen */
    'K', /* King */
};

enum class Player { None, White, Black };

struct Cell {
    Piece piece;
    Player player;

    Cell() : piece(Piece::None), player(Player::None) {}

    Cell(Piece piece_in, Player player_in)
        : piece(piece_in), player(player_in) {}
};

static const int BOARD_WIDTH = 8,
                 BOARD_HEIGHT = 8,
                 CELL_WIDTH = 7,
                 CELL_HEIGHT = 3;

using BoardT = array<array<Cell, BOARD_WIDTH>, BOARD_HEIGHT>;
void init_board(BoardT&);
void draw_board(const BoardT&);
