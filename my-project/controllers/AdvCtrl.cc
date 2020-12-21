//add definition of your processing function here
#include <cassert>
#include <map>
#include <json/json.h>
#include "AdvCtrl.h"
#include "models/Users.h"
using drogon::HttpStatusCode;
using drogon_model::sqlite3::Users;

static auto to_error(HttpStatusCode status_code, const std::string& reason) {
    static const std::map<HttpStatusCode, std::string> messages = {
        { HttpStatusCode::k400BadRequest, "Bad Request" },
        { HttpStatusCode::k404NotFound, "Not Found" },
    };
    Json::Value res;
    res["status"] = status_code;
    res["message"] = messages.at(status_code);
    res["reason"] = reason;
    auto res2 = drogon::toResponse(res);
    res2->setStatusCode(status_code);
    return res2;
}

void AdvCtrl::getAdv(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback, int num, const std::string& string) {
    Json::Value json;
    json["hello"] = "world";
    json["num"] = num;
    json["string"] = string;
    callback(drogon::toResponse(json));
}

void AdvCtrl::login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback) {
    const auto& json = req->getJsonObject();
    const auto& db = drogon::app().getDbClient();

    if (req->getMethod() == drogon::Post) {
        if (!json)
            return callback(to_error(HttpStatusCode::k400BadRequest, "bad json body"));
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

    const auto& username = req->session()->getOptional<std::string>("username");
    if (!username.has_value())
        return callback(drogon::toResponse(Json::Value::null));
    const auto& rows = db->execSqlSync(
        "SELECT * FROM users WHERE username=?",
        username);
    assert(rows.size() == 1);
    const auto& user = Users(rows[0]);
    callback(drogon::toResponse(user.toJson()));
}

void AdvCtrl::logout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback) {
    req->session()->erase("username");
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(HttpStatusCode::k204NoContent);
    callback(res);
}
