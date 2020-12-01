#include <ncurses.h>
#include "chess.hpp"
#include "draw.hpp"

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
