#include <cassert>
#include "api_AuthCtrl.h"
#include "models/User.h"
#include "utils.h"
using drogon::HttpResponse, drogon::HttpStatusCode;
using drogon_model::sqlite3::User;

void api::AuthCtrl::login(const HttpRequestPtr& req, Callback&& callback) {
    const auto& json = req->getJsonObject();
    const auto& db = drogon::app().getDbClient();

    if (req->getMethod() == drogon::Post) {
        // Post -> RequireJson
        // { "username": "..." }
        if (!json->isMember("username"))
            return callback(to_error(HttpStatusCode::k400BadRequest, "\"username\" not in request body"));
        const auto& username = (*json)["username"].asString();
        const auto& rows = db->execSqlSync("SELECT count(*) FROM users WHERE username=?", username);
        auto n_rows = rows[0]["count(*)"].as<int>();
        if (!n_rows)
            return callback(to_error(HttpStatusCode::k404NotFound, "unknown username"));
        assert(n_rows == 1);
        req->session()->insert("username", username);
    }

    // Get -> RequireAuth
    const auto& username = req->session()->get<std::string>("username");
    const auto& user = User::lookup_user(username);
    callback(drogon::toResponse(user->toJson()));
}

// Post -> RequireAuth
void api::AuthCtrl::logout(const HttpRequestPtr& req, Callback&& callback) {
    req->session()->erase("username");
    auto res = HttpResponse::newHttpResponse();
    res->setStatusCode(HttpStatusCode::k204NoContent);
    callback(res);
}

// Post -> RequireJson
// { "username": "..." }
void api::AuthCtrl::create(const HttpRequestPtr& req, Callback&& callback) {
    const auto& json = req->getJsonObject();
    const auto& db = drogon::app().getDbClient();
    if (!json->isMember("username"))
        return callback(to_error(HttpStatusCode::k400BadRequest, "\"username\" not in request body"));
    const auto& username = (*json)["username"].asString();
    if (User::lookup_user(username))
        return callback(to_error(HttpStatusCode::k409Conflict, "the given username already exists"));
    db->execSqlSync("INSERT INTO users(username) VALUES (?)", username);
    const auto& user = User::lookup_user(username);
    callback(drogon::toResponse(user->toJson()));
}
