#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr;
using drogon::Get;

namespace api {
class state : public drogon::HttpController<state> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(state::get, "/?stateId={1}", Get, "RequireAuth");
        METHOD_LIST_END

        void get(const HttpRequestPtr& req, Callback&& callback, const std::string& state_id_in);
};
}
