#include <cassert>
#include <drogon/HttpAppFramework.h>
#include "history.h"
#include "models/History.h"
#include "models/Game.h"
#include "utils.h"
using drogon::HttpStatusCode;
using drogon_model::sqlite3::History2, drogon_model::sqlite3::Game;

// Get -> RequireAuth, GameIdParam
void api::history::get(const HttpRequestPtr& req, Callback&& callback,
                       const std::string& game_id_in, const std::string& state_id_in) {
    uint64_t game_id = std::stoi(game_id_in);
    uint64_t state_id;

    try {
        state_id = std::stoi(state_id_in);
    } catch (const std::invalid_argument& err) {
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad state_id"));
    }

    const auto& history = History2::lookup_history(game_id, state_id);
    if (!history)
        return callback(to_error(HttpStatusCode::k404NotFound, "unknown state_id"));
    return callback(drogon::toResponse(history->toJson()));
}
