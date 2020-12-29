#include <cassert>
#include "game.h"
#include "models/User.h"
#include "models/Game.h"
#include "utils.h"
using drogon::HttpStatusCode;
using drogon_model::sqlite3::User, drogon_model::sqlite3::Game;

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

    const auto& db = drogon::app().getDbClient();
    db->execSqlSync("INSERT INTO games(white, black) VALUES (?, ?)", white, black);
    const auto& game = Game::last_insert_game();
    callback(drogon::toResponse(game.toJson()));
}
