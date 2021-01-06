#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr;
using drogon::Get;

namespace api {
class history : public drogon::HttpController<history> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(history::get, "/?gameId={1}&stateId={2}", Get, "RequireAuth", "GameIdParam");
        METHOD_LIST_END

        void get(const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in,
                 const std::string& state_id_in);
};
}
