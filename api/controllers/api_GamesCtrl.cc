#include <cassert>
#include "api_GamesCtrl.h"
#include "models/User.h"
#include "models/Game.h"
#include "utils.h"
using drogon::HttpStatusCode;
using drogon_model::sqlite3::User, drogon_model::sqlite3::Game;

// Post -> RequireAuth, RequireJson
// { "player": "(White|Black|Random)", "opponent": "other username" }
void api::GamesCtrl::new_game(const HttpRequestPtr& req, Callback&& callback) {
    const auto& json = req->getJsonObject();
    ASSERT_JSON_MEMBER(json, opponent);
    const auto& opponent = (*json)["opponent"].asString();
    if (!User::lookup_user(opponent))
        return callback(to_error(HttpStatusCode::k404NotFound, "unknown opponent"));
    const auto& username = req->session()->get<std::string>("username");
    if (username == opponent)
        return callback(to_error(HttpStatusCode::k400BadRequest,
                                 "can't play game against yourself"));
    ASSERT_JSON_MEMBER(json, player);
    const auto& player = (*json)["player"].asString();
    if (player != "White" && player != "Black" && player != "Random")
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad player"));
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
