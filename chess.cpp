#include <ncurses.h>
#include "chess.hpp"

static const int BOARD_WIDTH = 8,
                 BOARD_HEIGHT = 8,
                 CELL_WIDTH = 7,
                 CELL_HEIGHT = 3;

void
draw_board(void)
{
    // draw cells
    for (int cell_row = 0; cell_row < BOARD_HEIGHT; ++cell_row) {
        for (int cell_col = 0; cell_col < BOARD_WIDTH; ++cell_col) {
            const bool is_white = (cell_row + cell_col) % 2 == 0;
            const short color = is_white ? WhiteBlank : BlackBlank;
            const int x0 = cell_col * CELL_WIDTH,
                      y0 = cell_row * CELL_HEIGHT;

            for (int x1 = 0; x1 < CELL_WIDTH; ++x1) {
                for (int y1 = 0; y1 < CELL_HEIGHT; ++y1) {
                    move(y0 + y1, x0 + x1);
                    addch(' ' | COLOR_PAIR(color));
                }
            }
        }
    }

    attron(A_BOLD | A_DIM);

    // draw white pieces
    for (int cell_row = 0; cell_row < 2; ++cell_row) {
        for (int cell_col = 0; cell_col < BOARD_WIDTH; ++cell_col) {
            const bool cell_is_white = (cell_row + cell_col) % 2 != 0;
            const short color = cell_is_white ? WhiteOnWhite : WhiteOnBlack;
            const int x0 = cell_col * CELL_WIDTH,
                      y0 = cell_row * CELL_HEIGHT;
            move(y0 + CELL_HEIGHT / 2, x0 + CELL_WIDTH / 2);
            attron(COLOR_PAIR(color));
            addch('x');
            attroff(COLOR_PAIR(color));
        }
    }

    int cell_row = BOARD_HEIGHT;

    while (--cell_row >= BOARD_HEIGHT - 2) {
        for (int cell_col = 0; cell_col < BOARD_WIDTH; ++cell_col) {
            const bool cell_is_white = (cell_row + cell_col) % 2 != 0;
            const short color = cell_is_white ? BlackOnWhite : BlackOnBlack;
            const int x0 = cell_col * CELL_WIDTH,
                      y0 = cell_row * CELL_HEIGHT;
            move(y0 + CELL_HEIGHT / 2, x0 + CELL_WIDTH / 2);
            attron(COLOR_PAIR(color));
            addch('x');
            attroff(COLOR_PAIR(color));
        }
    }

    attroff(A_BOLD | A_DIM);
}
