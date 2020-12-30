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

// Get -> RequireAuth
void api::game::get_game(const HttpRequestPtr& req, Callback&& callback,
                         const std::string& game_id_in) {
    uint64_t game_id;

    try {
        game_id = std::stoi(game_id_in);
    } catch (const std::invalid_argument& err) {
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad game_id"));
    }

    const auto& game = Game::lookup_game(game_id);
    if (!game)
        return callback(to_error(HttpStatusCode::k404NotFound, "Unknown game_id"));
    const auto& username = req->session()->get<std::string>("username");
    if (username != *game->getWhite() && username != *game->getBlack())
        return callback(to_error(HttpStatusCode::k403Forbidden, "game is not owned by username"));
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

// Get -> RequireAuth
void api::game::get_moves(
        const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in,
        const std::string& row_in, const std::string& col_in) {
    uint64_t game_id;

    try {
        game_id = std::stoi(game_id_in);
    } catch (const std::invalid_argument& err) {
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad game_id"));
    }

    const auto& game = Game::lookup_game(game_id);
    if (!game)
        return callback(to_error(HttpStatusCode::k404NotFound, "Unknown game_id"));
    const auto& username = req->session()->get<std::string>("username");
    if (username != *game->getWhite() && username != *game->getBlack())
        return callback(to_error(HttpStatusCode::k403Forbidden, "game is not owned by username"));
    ChessState chess_state = State::lookup_state(*game->getStateid());

    size_t row;

    try {
        row = std::stoi(row_in);
    } catch (const std::invalid_argument& err) {
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad row"));
    }

    if (row >= Chess::BOARD_HEIGHT)
        return callback(to_error(HttpStatusCode::k400BadRequest, "row out of range"));

    size_t col;

    try {
        col = std::stoi(col_in);
    } catch (const std::invalid_argument& err) {
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad col"));
    }

    if (col >= Chess::BOARD_WIDTH)
        return callback(to_error(HttpStatusCode::k400BadRequest, "col out of range"));

    const auto& moves = Chess::get_possible_moves(chess_state.board, row, col);
    const auto& castles = Chess::get_possible_castles(chess_state.board, row, col,
                                                      chess_state.endgame_state);
    Json::Value json;
    json["moves"] = Chess::serialize_moves(moves);
    json["castles"] = Chess::serialize_castles(castles);
    callback(drogon::toResponse(json));
}
