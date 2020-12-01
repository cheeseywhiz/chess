// the abstract chess game
#pragma once
#include <array>
using std::array;

enum class Piece { None, Pawn, Knight, Bishop, Rook, Queen, King };

enum class Player { None, White, Black };

struct Cell {
    Piece piece;
    Player player;

    Cell() : piece(Piece::None), player(Player::None) {}

    Cell(Piece piece_in, Player player_in)
        : piece(piece_in), player(player_in) {}
};

static const int BOARD_WIDTH = 8, BOARD_HEIGHT = 8;
using BoardT = array<array<Cell, BOARD_WIDTH>, BOARD_HEIGHT>;
void init_board(BoardT&);
