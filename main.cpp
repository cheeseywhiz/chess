#include <vector>
#include <ncurses.h>
#include "chess.hpp"
#include "draw.hpp"
using std::vector;

int
main()
{
    init_ncurses();
    BoardT board;
    init_board(board);
    draw_board(board);
    State state = State::Ready;
    Player player = Player::White;
    draw_player(player);
    vector<Piece> white_captures, black_captures;
    size_t column1 = 0, row1 = 0, column2 = 0, row2 = 0, tmp;
    int c;

    while ((c = getch()) != ERR) {
        if (c == 'q') {
            endwin();
            return 0;
        }

        if (c == 27 /* ESC */) {
            state = State::Ready;
            draw_selection(state, column1, row1, column2);
            clear_possible_moves();
            continue;
        }

        switch (state) {
        case State::Ready:
            tmp = static_cast<size_t>(c) - 'a';
            if (tmp >= BOARD_WIDTH)
                break;
            column1 = tmp;
            state = State::Column1Selected;
            draw_selection(state, column1, row1, column2);
            break;
        case State::Column1Selected:
            tmp = '0' + BOARD_HEIGHT - static_cast<size_t>(c);
            if (tmp >= BOARD_HEIGHT)
                break;
            row1 = tmp;
            state = State::Row1Selected;
            draw_selection(state, column1, row1, column2);
            draw_possible_moves(board, row1, column1);
            break;
        case State::Row1Selected:
            tmp = static_cast<size_t>(c) - 'a';
            if (tmp >= BOARD_WIDTH)
                break;
            column2 = tmp;
            state = State::Column2Selected;
            draw_selection(state, column1, row1, column2);
            break;
        case State::Column2Selected: {
            tmp = '0' + BOARD_HEIGHT - static_cast<size_t>(c);
            if (tmp >= BOARD_HEIGHT)
                break;
            row2 = tmp;
            MoveResult result = \
                do_move(board, player, row1, column1, row2, column2);
            Player new_player = player;

            if (result.did_move) {
                draw_board(board);
                new_player = player == Player::White
                    ? Player::Black
                    : Player::White;
                draw_player(new_player);
            }

            if (result.capture != Piece::None) {
                if (player == Player::White) {
                    white_captures.push_back(result.capture);
                } else {
                    black_captures.push_back(result.capture);
                }

                draw_captures(white_captures, black_captures);
            }

            state = State::Ready;
            draw_selection(state, column1, row1, column2);
            clear_possible_moves();
            player = new_player;
            break; }
        default:
            break;
        }
    }

    return 1;
}
