#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr, drogon::HttpResponsePtr;
using drogon::Get, drogon::Post;

namespace api {
class game : public drogon::HttpController<game> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(game::new_game, "/new_game", Post, "RequireAuth", "RequireJson");
        METHOD_ADD(game::get_game, "/{1}", Get, "RequireAuth");
        METHOD_LIST_END

        void new_game(const HttpRequestPtr& req, Callback&& callback);
        void get_game(const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in);
};
}
