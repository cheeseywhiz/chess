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
    ChessState new_state;
    int c;

    while ((c = getch()) != ERR) {
        if (c == 'Q') {
            endwin();
            return 0;
        }

        assert(!history.empty());

        if (c == 27) {  // ESC
            switch (history.top().state) {
            case State::Promotion:
                history.top().state = State::Ready;
                draw_board(history.top().board);
                draw_player(history.top().player, history.top().endgame_state);
                break;
            case State::Column1Selected:
            case State::Row1Selected:
            case State::Column2Selected:
                history.top().state = State::Ready;
                draw_selection();
                clear_possible_moves();
                break;
            case State::Ready:
            case State::Endgame:
                break;
            }
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
            draw_selection();
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
            draw_selection();
            history.push(std::move(state));
            continue;
        }

        switch (history.top().state) {
        case State::Ready:
            tmp = static_cast<size_t>(c) - 'a';
            if (tmp >= BOARD_WIDTH)
                break;
            column1 = tmp;
            history.top().state = State::Column1Selected;
            draw_selection(column1);
            break;
        case State::Column1Selected:
            tmp = '0' + BOARD_HEIGHT - static_cast<size_t>(c);
            if (tmp >= BOARD_HEIGHT)
                break;
            row1 = tmp;

            if (history.top().board[row1][column1].player
                    != history.top().player) {
                history.top().state = State::Ready;
                draw_selection();
                clear_possible_moves();
                break;
            }

            history.top().state = State::Row1Selected;
            draw_selection(column1, row1);
            draw_possible_moves(
                history.top().board, history.top().endgame_state, row1, column1);
            break;
        case State::Row1Selected:
            tmp = static_cast<size_t>(c) - 'a';
            if (tmp >= BOARD_WIDTH)
                break;
            column2 = tmp;
            history.top().state = State::Column2Selected;
            draw_selection(column1, row1, column2);
            break;
        case State::Column2Selected: {
            tmp = '0' + BOARD_HEIGHT - static_cast<size_t>(c);
            if (tmp >= BOARD_HEIGHT)
                break;
            row2 = tmp;
            history.top().state = State::Ready;
            draw_selection();
            clear_possible_moves();
            new_state = history.top();
            MoveResult result = \
                do_move(new_state.board, new_state.endgame_state,
                        new_state.player, row1, column1, row2, column2);
            if (!result.did_move)
                break;
            draw_board(new_state.board);
            Player player = new_state.player;
            Player new_player = player == Player::White
                ? Player::Black
                : Player::White;
            new_state.endgame_state = \
                get_endgame_state(new_state.board, new_player);
            if (new_state.endgame_state == EndgameState::Checkmate
                    || new_state.endgame_state == EndgameState::Stalemate)
                new_state.state = State::Endgame;

            if (result.capture != Piece::None) {
                if (player == Player::White) {
                    new_state.white_captures.push_back(result.capture);
                } else {
                    new_state.black_captures.push_back(result.capture);
                }

                draw_captures(new_state.white_captures,
                              new_state.black_captures);
            }

            if (result.can_promote) {
                history.top().state = new_state.state = State::Promotion;
                draw_promotion_prompt(player);
            } else {
                new_state.player = new_player;
                draw_player(new_player, new_state.endgame_state);
                history.push(std::move(new_state));
                while (!backtrace.empty())
                    backtrace.pop();
            }

            break; }
        case State::Promotion: {
            Piece piece = Piece::None;

            switch (c) {
            case 'n':
                piece = Piece::Knight;
                break;
            case 'b':
                piece = Piece::Bishop;
                break;
            case 'r':
                piece = Piece::Rook;
                break;
            case 'q':
                piece = Piece::Queen;
                break;
            }

            if (piece == Piece::None)
                break;
            new_state.state = State::Ready;
            new_state.board[row2][column2].piece = piece;
            draw_board(new_state.board);
            new_state.player = new_state.player == Player::White
                ? Player::Black
                : Player::White;
            new_state.endgame_state = \
                get_endgame_state(new_state.board, new_state.player);
            if (new_state.endgame_state == EndgameState::Checkmate
                    || new_state.endgame_state == EndgameState::Stalemate)
                new_state.state = State::Endgame;
            draw_player(new_state.player, new_state.endgame_state);
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
