//add definition of your processing function here
#include <json/json.h>
#include "AdvCtrl.h"
using drogon::HttpStatusCode;

void AdvCtrl::getAdv(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback, int num, std::string string) {
    Json::Value json;
    json["hello"] = "world";
    json["num"] = num;
    json["string"] = string;
    callback(drogon::toResponse(json));
}

void AdvCtrl::login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback) {
    const auto& json = req->getJsonObject();

    if (req->getMethod() == drogon::Post) {
        if (!json) {
            Json::Value res;
            res["status"] = HttpStatusCode::k400BadRequest;
            res["message"] = "Bad Request";
            res["reason"] = "bad json body";
            auto res2 = drogon::toResponse(res);
            res2->setStatusCode(HttpStatusCode::k400BadRequest);
            return callback(res2);
        }

        if (!json->isMember("username")) {
            Json::Value res;
            res["status"] = HttpStatusCode::k400BadRequest;
            res["message"] = "Bad Request";
            res["reason"] = "\"username\" not in request body";
            auto res2 = drogon::toResponse(res);
            res2->setStatusCode(HttpStatusCode::k400BadRequest);
            return callback(res2);
        }

        req->session()->insert("username", (*json)["username"].asString());
    }

    Json::Value res;
    const auto& username = req->session()->getOptional<std::string>("username");
    res["username"] = username.has_value() ? *username : Json::Value::null;
    callback(drogon::toResponse(res));
}

void AdvCtrl::logout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback) {
    req->session()->erase("username");
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(HttpStatusCode::k204NoContent);
    callback(res);
}
