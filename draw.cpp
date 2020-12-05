// draw the chess game state
#include <unistd.h>
#include <ncurses.h>
#include "draw.hpp"
#include "chess.hpp"

static const short WhiteOnWhite = 1,
                   WhiteOnBlack = 2,
                   BlackOnWhite = 3,
                   BlackOnBlack = 4;
static const short WhiteBlank = BlackOnWhite,
                   BlackBlank = WhiteOnBlack;

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

static const unsigned piece_char[] = {
    ' ', /* None */
    'p', /* Pawn */
    'N', /* Knight */
    'B', /* Bishop */
    'R', /* Rook */
    'Q', /* Queen */
    'K', /* King */
};

void
draw_board(const BoardT& board)
{
    // draw top letter axis
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        move(1, static_cast<int>(col * CELL_WIDTH + 1 + CELL_WIDTH / 2));
        addch('a' + static_cast<int>(col));
    }

    // draw cells
    for (size_t row = 0; row < BOARD_HEIGHT; ++row) {
        // draw left number axis
        move(static_cast<int>(row * CELL_HEIGHT + CELL_HEIGHT / 2) + 2, 0);
        addch('0' + BOARD_HEIGHT - static_cast<int>(row));

        for (size_t col = 0; col < BOARD_WIDTH; ++col) {
            const bool cell_is_white = (row + col) % 2 == 0;
            short color = cell_is_white ? WhiteBlank : BlackBlank;
            const size_t x0 = col * CELL_WIDTH + 1,
                         y0 = row * CELL_HEIGHT + 2;

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

        // draw right number axis
        move(static_cast<int>(row * CELL_HEIGHT + CELL_HEIGHT / 2) + 2,
             static_cast<int>(BOARD_WIDTH * CELL_WIDTH) + 1);
        addch('0' + BOARD_HEIGHT - static_cast<int>(row));
    }

    // draw bottom letter axis
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        move(2 + BOARD_HEIGHT * CELL_HEIGHT,
             static_cast<int>(col * CELL_WIDTH + 1 + CELL_WIDTH / 2));
        addch('a' + static_cast<int>(col));
    }
}

static void draw_captures(Player, const vector<Piece>&);

void
draw_captures(const vector<Piece>& white_captures,
              const vector<Piece>& black_captures)
{
    draw_captures(Player::White, white_captures);
    draw_captures(Player::Black, black_captures);
}

static void
draw_captures(Player player, const vector<Piece>& captures)
{
    int row;
    short color;

    if (player == Player::White) {
        row = 2 + BOARD_HEIGHT * CELL_HEIGHT + 1;
        color = BlackOnWhite;
    } else {
        row = 0;
        color = WhiteOnBlack;
    }

    move(row, 1);
    attron(COLOR_PAIR(color) | A_BOLD | A_DIM);
    for (Piece piece : captures)
        printw(" %c ", piece_char[static_cast<int>(piece)]);
    attroff(COLOR_PAIR(color) | A_BOLD | A_DIM);
}

void
draw_player(Player player)
{
    move(2 + BOARD_HEIGHT * CELL_HEIGHT + 2, 1);
    clrtoeol();
    printw("%s", player == Player::White ? "white" : "black");
}

void
draw_selection(State state, size_t column1, size_t row1, size_t column2)
{
    move(2 + BOARD_HEIGHT * CELL_HEIGHT + 3, 1);
    clrtoeol();
    if (state == State::Column1Selected
            || state == State::Row1Selected
            || state == State::Column2Selected)
        printw("%c", static_cast<char>(column1) + 'a');
    if (state == State::Row1Selected
            || state == State::Column2Selected)
        printw("%c", '0' + BOARD_HEIGHT - static_cast<char>(row1));
    if (state == State::Column2Selected)
        printw("%c", static_cast<char>(column2) + 'a');
}
