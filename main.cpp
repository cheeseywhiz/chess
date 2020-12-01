#include <unistd.h>
#include <ncurses.h>
#include "chess.hpp"

static void init_ncurses(void);

int
main()
{
    init_ncurses();
    hello();
    endwin();
}

void
init_ncurses(void)
{
    initscr();
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
}
