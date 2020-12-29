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
    uint64_t game_id = std::stoi(game_id_in);
    const auto& game = Game::lookup_game(game_id);
    if (!game)
        return callback(to_error(HttpStatusCode::k404NotFound, "Unknown game_id"));
    const auto& username = req->session()->get<std::string>("username");
    if (username != *game->getWhite() && username != *game->getBlack())
        return callback(to_error(HttpStatusCode::k403Forbidden, "game is not owned by username"));
    Json::Value json = game->toJson();
    uint64_t state_id = *game->getStateId();
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
