#include <cassert>
#include <drogon/HttpAppFramework.h>
#include "state.h"
#include "models/State.h"
#include "utils.h"
#include "serialize.h"
using drogon::HttpStatusCode;
using drogon_model::sqlite3::State;

// Get -> RequireAuth, GameIdParam
void api::state::get(const HttpRequestPtr& req, Callback&& callback,
                     const std::string& state_id_in) {
    uint64_t state_id;

    try {
        state_id = std::stoi(state_id_in);
    } catch (const std::invalid_argument& err) {
        return callback(to_error(HttpStatusCode::k400BadRequest, "bad state_id"));
    }

    const auto& state = State::lookup_state(state_id);
    if (!state)
        return callback(to_error(HttpStatusCode::k404NotFound, "unknown state_id"));
    Chess::SerializedState serialized_state(*state);
    return callback(drogon::toResponse(serialized_state.to_json(state_id)));
}
