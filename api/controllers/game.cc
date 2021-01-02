#include <cassert>
#include "game.h"
#include "models/User.h"
#include "models/Game.h"
#include "models/State.h"
#include "utils.h"
#include "serialize.h"
using drogon::HttpStatusCode;
using drogon_model::sqlite3::User, drogon_model::sqlite3::Game, drogon_model::sqlite3::State;
using Chess::SerializedState;

// Get -> RequireAuth, GameIdParam
void api::game::get_game(const HttpRequestPtr& req, Callback&& callback,
                         const std::string& game_id_in) {
    uint64_t game_id = std::stoi(game_id_in);
    const auto& game = Game::lookup_game(game_id);
    assert(game);
    const auto& username = req->session()->get<std::string>("username");
    assert(username == *game->getWhite() || username == *game->getBlack());
    Json::Value json = game->toJson();
    uint64_t state_id = *game->getStateid();
    ChessState chess_state = State::lookup_state(state_id);
    SerializedState serialized_state(chess_state);
    json["state"] = serialized_state.to_json();
    callback(drogon::toResponse(json));
}

// Post -> RequireAuth, RequireJson
// { "player": "(White|Black|Random)", "opponent": "other username" }
void api::game::new_game(const HttpRequestPtr& req, Callback&& callback) {
    const auto& json = req->getJsonObject();
    ASSERT_JSON_MEMBER(json, player);
    const auto& player = (*json)["player"].asString();
    if (player != "White" && player != "Black" && player != "Random")
        return callback(to_error(HttpStatusCode::k400BadRequest, "Must choose a player"));
    ASSERT_JSON_MEMBER(json, opponent);
    const auto& opponent = (*json)["opponent"].asString();
    if (opponent.empty())
        return callback(to_error(HttpStatusCode::k400BadRequest,
                                 "Please enter an opponent username"));
    if (!User::lookup_user(opponent))
        return callback(to_error(HttpStatusCode::k404NotFound, "Unknown opponent username"));
    const auto& username = req->session()->get<std::string>("username");
    if (username == opponent)
        return callback(to_error(HttpStatusCode::k400BadRequest,
                                 "The opponent can't be yourself"));
    std::string white, black;

    if (player == "White" || player == "Random") {
        white = username;
        black = opponent;
    } else { // player == "Black"
        white = opponent;
        black = username;
    }

    const auto& game = Game::create_new_game(white, black);
    const auto& response = drogon::toResponse(game.toJson());
    response->setStatusCode(HttpStatusCode::k201Created);
    callback(response);
}

// Get -> RequireAuth, GameIdParam, CellParams
void api::game::get_moves(
        const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in,
        const std::string& row_in, const std::string& col_in) {
    uint64_t game_id = std::stoi(game_id_in);
    const auto& game = Game::lookup_game(game_id);
    assert(game);
    const auto& username = req->session()->get<std::string>("username");
    assert(username == *game->getWhite() || username == *game->getBlack());
    ChessState chess_state = State::lookup_state(*game->getStateid());
    const auto& player = chess_state.player == Chess::Player::White
        ? *game->getWhite()
        : *game->getBlack();
    if (username != player)
        return callback(to_error(HttpStatusCode::k403Forbidden, "not your turn"));
    size_t row = std::stoi(row_in), col = std::stoi(col_in);
    assert(row < Chess::BOARD_HEIGHT && col < Chess::BOARD_WIDTH);
    const auto& cell = chess_state.board[row][col];
    if (cell.player == Chess::Player::None)
        return callback(to_error(HttpStatusCode::k400BadRequest, "player is None"));
    if (cell.player != chess_state.player)
        return callback(to_error(HttpStatusCode::k400BadRequest, "not that player's turn"));

    auto moves = Chess::get_possible_moves(chess_state.board, row, col);
    const auto& castles = Chess::get_possible_castles(chess_state.board, row, col,
                                                      chess_state.endgame_state);
    size_t home_row = chess_state.board[row][col].player == Chess::Player::White
        ? Chess::BOARD_HEIGHT - 1
        : 0;
    if (castles & Chess::Castles::Queen)
        moves.emplace_back(home_row, 2);
    if (castles & Chess::Castles::King)
        moves.emplace_back(home_row, Chess::BOARD_WIDTH - 2);
    Json::Value json;
    json["moves"] = Chess::serialize_moves(moves);
    callback(drogon::toResponse(json));
}

// Post -> RequireAuth, GameIdParam, CellParams, MoveParams
void api::game::do_move(
        const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in,
        const std::string& row1_in, const std::string& col1_in,
        const std::string& row2_in, const std::string& col2_in) {
    uint64_t game_id = std::stoi(game_id_in);
    const auto& game = Game::lookup_game(game_id);
    assert(game);
    const auto& username = req->session()->get<std::string>("username");
    assert(username == *game->getWhite() || username == *game->getBlack());
    ChessState chess_state = State::lookup_state(*game->getStateid());
    const auto& playerUsername = chess_state.player == Chess::Player::White
        ? *game->getWhite()
        : *game->getBlack();
    if (username != playerUsername)
        return callback(to_error(HttpStatusCode::k403Forbidden, "not your turn"));
    if (chess_state.state != Chess::State::Ready)
        return callback(to_error(HttpStatusCode::k409Conflict, "not ready"));
    size_t row1 = std::stoi(row1_in), col1 = std::stoi(col1_in),
           row2 = std::stoi(row2_in), col2 = std::stoi(col2_in);
    assert(row1 < Chess::BOARD_HEIGHT && col1 < Chess::BOARD_WIDTH);
    assert(row2 < Chess::BOARD_HEIGHT && col2 < Chess::BOARD_WIDTH);
    ChessState new_state = chess_state;
    const auto& result = Chess::do_move(new_state.board, new_state.endgame_state, new_state.player,
                                        row1, col1, row2, col2);
    if (!result.did_move)
        return;
    auto player = new_state.player;
    auto new_player = player == Chess::Player::White
        ? Chess::Player::Black
        : Chess::Player::White;
    new_state.endgame_state = Chess::get_endgame_state(new_state.board, new_player);
    if (new_state.endgame_state == Chess::EndgameState::Checkmate ||
            new_state.endgame_state == Chess::EndgameState::Stalemate)
        new_state.state = Chess::State::Endgame;

    if (result.capture != Chess::Piece::None) {
        if (player == Chess::Player::White) {
            new_state.white_captures.push_back(result.capture);
        } else {
            new_state.black_captures.push_back(result.capture);
        }
    }

    if (result.can_promote)
        return callback(to_error(HttpStatusCode::k501NotImplemented, "can't promote"));

    new_state.player = new_player;
    if (new_state.player == Chess::Player::White)
        ++new_state.n_moves;
    uint64_t new_state_id = State::insert_state(new_state);
    game->history_push(new_state_id);
    SerializedState serialized_state(new_state);
    return callback(drogon::toResponse(serialized_state.to_json()));
}
