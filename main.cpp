#include <cassert>
#include <stack>
#include <ncurses.h>
#include "chess.hpp"
#include "draw.hpp"
using std::stack;

int
main()
{
    init_ncurses();
    stack<ChessState> history, backtrace;
    history.emplace();
    draw_board(history.top().board);
    draw_player(history.top().player, history.top().endgame_state);
    size_t column1 = 0, row1 = 0, column2 = 0, row2 = 0, tmp;
    int c;

    while ((c = getch()) != ERR) {
        if (c == 'q') {
            endwin();
            return 0;
        }

        if (c == 27 /* ESC */) {
            history.top().state = State::Ready;
            draw_selection(history.top().state, column1, row1, column2);
            clear_possible_moves();
            continue;
        }

        if (c == KEY_LEFT) {
            // backwards through history
            if (history.size() <= 1)
                continue;
            ChessState state = std::move(history.top());
            state.state = State::Ready;
            history.pop();
            backtrace.push(std::move(state));
            draw_board(history.top().board);
            clear_possible_moves();
            draw_captures(history.top().white_captures,
                          history.top().black_captures);
            draw_player(history.top().player, history.top().endgame_state);
            draw_selection(history.top().state, column1, row1, column2);
            continue;
        }

        if (c == KEY_RIGHT) {
            // forwards through history
            if (backtrace.empty())
                continue;
            ChessState state = std::move(backtrace.top());
            backtrace.pop();
            draw_board(state.board);
            clear_possible_moves();
            draw_captures(state.white_captures, state.black_captures);
            draw_player(state.player, state.endgame_state);
            draw_selection(state.state, column1, row1, column2);
            history.push(std::move(state));
            continue;
        }

        assert(!history.empty());

        switch (history.top().state) {
        case State::Ready:
            tmp = static_cast<size_t>(c) - 'a';
            if (tmp >= BOARD_WIDTH)
                break;
            column1 = tmp;
            history.top().state = State::Column1Selected;
            draw_selection(history.top().state, column1, row1, column2);
            break;
        case State::Column1Selected:
            tmp = '0' + BOARD_HEIGHT - static_cast<size_t>(c);
            if (tmp >= BOARD_HEIGHT)
                break;
            row1 = tmp;

            if (history.top().board[row1][column1].player
                    != history.top().player) {
                history.top().state = State::Ready;
                draw_selection(history.top().state, column1, row1, column2);
                clear_possible_moves();
                break;
            }

            history.top().state = State::Row1Selected;
            draw_selection(history.top().state, column1, row1, column2);
            draw_possible_moves(history.top().board, row1, column1);
            break;
        case State::Row1Selected:
            tmp = static_cast<size_t>(c) - 'a';
            if (tmp >= BOARD_WIDTH)
                break;
            column2 = tmp;
            history.top().state = State::Column2Selected;
            draw_selection(history.top().state, column1, row1, column2);
            break;
        case State::Column2Selected: {
            tmp = '0' + BOARD_HEIGHT - static_cast<size_t>(c);
            if (tmp >= BOARD_HEIGHT)
                break;
            row2 = tmp;
            history.top().state = State::Ready;
            draw_selection(history.top().state, column1, row1, column2);
            clear_possible_moves();
            ChessState new_state = history.top();
            MoveResult result = \
                do_move(new_state.board, new_state.player, row1,
                        column1, row2, column2);
            if (!result.did_move)
                break;
            Player player = new_state.player;
            new_state.player = player == Player::White
                ? Player::Black
                : Player::White;
            new_state.endgame_state = \
                get_endgame_state(new_state.board, new_state.player);
            if (new_state.endgame_state == EndgameState::Checkmate
                    || new_state.endgame_state == EndgameState::Stalemate)
                new_state.state = State::Endgame;
            draw_board(new_state.board);
            draw_player(new_state.player, new_state.endgame_state);

            if (result.capture != Piece::None) {
                if (player == Player::White) {
                    new_state.white_captures.push_back(result.capture);
                } else {
                    new_state.black_captures.push_back(result.capture);
                }

                draw_captures(new_state.white_captures,
                              new_state.black_captures);
            }

            history.push(std::move(new_state));
            while (!backtrace.empty())
                backtrace.pop();
            break; }
        case State::Endgame:
            break;
        }
    }

    return 1;
}
