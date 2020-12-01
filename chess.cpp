#include <ncurses.h>
#include "chess.hpp"

static void init_player_pieces(BoardT&, Player);

void
init_board(BoardT& board)
{
    init_player_pieces(board, Player::White);
    init_player_pieces(board, Player::Black);
}

static void
init_player_pieces(BoardT& board, Player player)
{
    size_t row = player == Player::White ? BOARD_HEIGHT - 2 : 1;
    for (size_t col = 0; col < BOARD_WIDTH; ++col)
        board[row][col] = Cell(Piece::Pawn, player);
    row = player == Player::White ? BOARD_HEIGHT - 1 : 0;
    board[row][0] = board[row][BOARD_WIDTH - 1] = Cell(Piece::Rook, player);
    board[row][1] = board[row][BOARD_WIDTH - 2] = Cell(Piece::Knight, player);
    board[row][2] = board[row][BOARD_WIDTH - 3] = Cell(Piece::Bishop, player);
    board[row][3] = Cell(Piece::Queen, player);
    board[row][4] = Cell(Piece::King, player);
}

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
