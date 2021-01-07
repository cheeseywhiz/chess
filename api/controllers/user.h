#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr, drogon::HttpResponsePtr;
using drogon::Get, drogon::Post;

namespace api {
class user : public drogon::HttpController<user> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(user::login, "/login", Get, "RequireAuth");
        METHOD_ADD(user::login, "/login", Post, "RequireJson");
        METHOD_ADD(user::logout, "/logout", Post, "RequireAuth");
        METHOD_ADD(user::create, "/create", Post, "RequireJson");
        METHOD_ADD(user::games, "/games", Get, "RequireAuth");
        METHOD_LIST_END

        void login(const HttpRequestPtr& req, Callback&& callback);
        void logout(const HttpRequestPtr& req, Callback&& callback);
        void create(const HttpRequestPtr& req, Callback&& callback);
        void games(const HttpRequestPtr& req, Callback&& callback);
};
}
