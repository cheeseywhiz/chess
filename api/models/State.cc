#include <drogon/HttpAppFramework.h>
#include "State.h"
#include "utils.h"
#include "serialize.h"
using Chess::ChessState, Chess::SerializedState;

namespace drogon_model {
namespace sqlite3 {
ChessState State::lookup_state(uint64_t state_id) {
    const auto& db = drogon::app().getDbClient();
    const auto& rows = db->execSqlSync("SELECT * FROM states WHERE state_id = ?", state_id);
    assert(rows.size() == 1);
    const State orm_state(rows[0]);
    SerializedState serialized_state(
        *orm_state.getState(), *orm_state.getPlayer(), *orm_state.getEndgameState(),
        *orm_state.getNMoves(), *orm_state.getWhiteCaptures(), *orm_state.getBlackCaptures(),
        *orm_state.getBoard());
    return serialized_state.deserialize();
}

uint64_t State::insert_state(const ChessState& chess_state) {
    const auto& db = drogon::app().getDbClient();
    SerializedState serialized_state(chess_state);
    db->execSqlSync(
        "INSERT INTO "
        "states (state, player, endgame_state, n_moves, white_captures, black_captures, board) "
        " VALUES (?, ?, ?, ?, ?, ?, ?)",
        serialized_state.state, serialized_state.player, serialized_state.endgame_state,
        serialized_state.n_moves, serialize_json(serialized_state.white_captures),
        serialize_json(serialized_state.black_captures), serialize_json(serialized_state.board));
    return last_insert_rowid();
}
} // namespace sqlite3
} // namespace drogon_model
