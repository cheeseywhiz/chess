#include <cassert>
#include <json/json.h>
#include "user.h"
#include "models/User.h"
#include "models/Game.h"
#include "utils.h"
using drogon::HttpResponse, drogon::HttpStatusCode;
using drogon_model::sqlite3::User, drogon_model::sqlite3::Game;

void api::user::login(const HttpRequestPtr& req, Callback&& callback) {
    if (req->getMethod() == drogon::Post) {
        // Post -> RequireJson
        // { "username": "..." }
        const auto& json = req->getJsonObject();
        ASSERT_JSON_MEMBER(json, username);
        const auto& username = (*json)["username"].asString();
        if (!User::lookup_user(username))
            return callback(to_error(HttpStatusCode::k404NotFound, "Unknown username"));
        req->session()->insert("username", username);
    }

    // Get -> RequireAuth
    const auto& username = req->session()->get<std::string>("username");
    const auto& user = User::lookup_user(username);
    assert(user);
    callback(drogon::toResponse(user->toJson()));
}

// Post -> RequireAuth
void api::user::logout(const HttpRequestPtr& req, Callback&& callback) {
    req->session()->erase("username");
    auto res = HttpResponse::newHttpResponse();
    res->setStatusCode(HttpStatusCode::k204NoContent);
    callback(res);
}

// Post -> RequireJson
// { "username": "..." }
void api::user::create(const HttpRequestPtr& req, Callback&& callback) {
    const auto& json = req->getJsonObject();
    const auto& db = drogon::app().getDbClient();
    ASSERT_JSON_MEMBER(json, username);
    const auto& username = (*json)["username"].asString();
    if (username.empty())
        return callback(to_error(HttpStatusCode::k400BadRequest,
                                 "Please enter a username"));
    if (User::lookup_user(username))
        return callback(to_error(HttpStatusCode::k409Conflict,
                                 "Username already exists"));
    db->execSqlSync("INSERT INTO users(username) VALUES (?)", username);
    const auto& user = User::lookup_user(username);
    callback(drogon::toResponse(user->toJson()));
}

// Get -> RequireAuth
void api::user::games(const HttpRequestPtr& req, Callback&& callback) {
    const auto& username = req->session()->get<std::string>("username");
    const auto& user = User::lookup_user(username);
    assert(user);
    const auto& games = Game::lookup_user_games(*user);
    Json::Value games_json(Json::ValueType::arrayValue);
    for (const auto& game : games)
        games_json.append(game.serialize());
    return callback(drogon::toResponse(games_json));
}
