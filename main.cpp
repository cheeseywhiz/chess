#include <unistd.h>
#include <ncurses.h>
#include "chess.hpp"

static void init_ncurses(void);

int
main()
{
    init_ncurses();
    BoardT board;
    init_board(board);
    draw_board(board);
    getch();
    endwin();
}

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
