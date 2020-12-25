#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr, drogon::HttpResponsePtr;
using /*drogon::Get,*/ drogon::Post;

namespace api {
class GamesCtrl : public drogon::HttpController<GamesCtrl> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(GamesCtrl::new_game, "/new_game", Post, "RequireAuth", "RequireJson");
        METHOD_LIST_END

        void new_game(const HttpRequestPtr& req, Callback&& callback);
};
}
