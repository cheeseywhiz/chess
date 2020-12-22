#include <cassert>
#include "api_AuthCtrl.h"
#include "models/User.h"
#include "utils.h"
using drogon::HttpResponse, drogon::HttpStatusCode;
using drogon_model::sqlite3::User;

void api::AuthCtrl::login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback) {
    const auto& json = req->getJsonObject();
    const auto& db = drogon::app().getDbClient();

    if (req->getMethod() == drogon::Post) {
        // Post -> RequireJson
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
    const auto& username = req->session()->getOptional<std::string>("username");
    const auto& rows = db->execSqlSync(
        "SELECT * FROM users WHERE username=?",
        username);
    assert(rows.size() == 1);
    const auto& user = User(rows[0]);
    callback(drogon::toResponse(user.toJson()));
}

// RequireAuth
void api::AuthCtrl::logout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback) {
    req->session()->erase("username");
    auto res = HttpResponse::newHttpResponse();
    res->setStatusCode(HttpStatusCode::k204NoContent);
    callback(res);
}
