#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr, drogon::HttpResponsePtr;
using drogon::Get, drogon::Post;

namespace api {
class AuthCtrl:public drogon::HttpController<AuthCtrl> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(AuthCtrl::login, "/login", Get, "RequireAuth");
        METHOD_ADD(AuthCtrl::login, "/login", Post, "RequireJson");
        METHOD_ADD(AuthCtrl::logout, "/logout", Post, "RequireAuth");
        METHOD_ADD(AuthCtrl::create, "/create", Post, "RequireJson");
        METHOD_LIST_END

        void login(const HttpRequestPtr& req, Callback&& callback);
        void logout(const HttpRequestPtr& req, Callback&& callback);
        void create(const HttpRequestPtr& req, Callback&& callback);
};
}
