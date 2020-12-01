// draw the chess game state
#include <unistd.h>
#include <ncurses.h>
#include "draw.hpp"
#include "chess.hpp"

void
init_ncurses(void)
{
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

#ifdef DEBUG
    // print the PID so that we can connect in another terminal with GDB
    printw(" %d", getpid());
    getch();

    // clear what we just wrote
    move(0, 0);
    clrtoeol();
#endif

    const short fg_white = COLOR_WHITE,
                bg_white = 8,
                fg_black = COLOR_BLACK,
                bg_black = 9;
    init_pair(WhiteOnWhite, fg_white, bg_white);
    init_pair(WhiteOnBlack, fg_white, bg_black);
    init_pair(BlackOnWhite, fg_black, bg_white);
    init_pair(BlackOnBlack, fg_black, bg_black);
}

static const int CELL_WIDTH = 7, CELL_HEIGHT = 3;

void
draw_board(const BoardT& board)
{
    // draw cells
    for (size_t row = 0; row < BOARD_HEIGHT; ++row) {
        move(static_cast<int>(row * CELL_HEIGHT + CELL_HEIGHT / 2), 0);
        addch('8' - static_cast<int>(row));

        for (size_t col = 0; col < BOARD_WIDTH; ++col) {
            const bool cell_is_white = (row + col) % 2 == 0;
            short color = cell_is_white ? WhiteBlank : BlackBlank;
            const size_t x0 = col * CELL_WIDTH + 1,
                         y0 = row * CELL_HEIGHT;

            for (size_t x1 = 0; x1 < CELL_WIDTH; ++x1) {
                for (size_t y1 = 0; y1 < CELL_HEIGHT; ++y1) {
                    move(static_cast<int>(y0 + y1), static_cast<int>(x0 + x1));
                    addch(' ' | COLOR_PAIR(color));
                }
            }

            const Cell& cell = board[row][col];
            if (cell.piece == Piece::None)
                continue;

            if (cell.player == Player::White) {
                color = cell_is_white ? WhiteOnWhite : WhiteOnBlack;
            } else {
                color = cell_is_white ? BlackOnWhite : BlackOnBlack;
            }

            move(static_cast<int>(y0 + CELL_HEIGHT / 2),
                 static_cast<int>(x0 + CELL_WIDTH / 2));
            addch(piece_char[static_cast<int>(cell.piece)]
                  | COLOR_PAIR(color) | A_BOLD | A_DIM);
        }
    }

    for (int col = 0; col < BOARD_WIDTH; ++col) {
        move(BOARD_HEIGHT * CELL_HEIGHT,
             static_cast<int>(col * CELL_WIDTH + 1 + CELL_WIDTH / 2));
        addch('a' + static_cast<int>(col));
    }
}
