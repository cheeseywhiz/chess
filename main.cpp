#include <unistd.h>
#include <ncurses.h>
#include "chess.hpp"

static void init_ncurses(void);

int
main()
{
    init_ncurses();
    draw_board();
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

    init_pair(WhiteOnWhite, COLOR_WHITE, COLOR_BLACK);
    init_pair(WhiteOnBlack, COLOR_WHITE, COLOR_BLACK);
    init_pair(BlackOnWhite, COLOR_BLACK, COLOR_WHITE);
    init_pair(BlackOnBlack, COLOR_BLACK, COLOR_WHITE);
    init_pair(WhiteBlank, COLOR_BLACK, COLOR_WHITE);
    init_pair(BlackBlank, COLOR_WHITE, COLOR_BLACK);
}
