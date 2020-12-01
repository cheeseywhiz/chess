// draw the chess game state
#pragma once
#include "chess.hpp"

static const short WhiteOnWhite = 1,
                   WhiteOnBlack = 2,
                   BlackOnWhite = 3,
                   BlackOnBlack = 4;
static const short WhiteBlank = BlackOnWhite,
                   BlackBlank = WhiteOnBlack;

static const unsigned piece_char[] = {
    ' ', /* None */
    'p', /* Pawn */
    'N', /* Knight */
    'B', /* Bishop */
    'R', /* Rook */
    'Q', /* Queen */
    'K', /* King */
};

void init_ncurses(void);
void draw_board(const BoardT&);
