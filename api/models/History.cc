#include <drogon/HttpAppFramework.h>
#include "History.h"
#include "Game.h"

namespace drogon_model {
namespace sqlite3 {
void History2::push(const Game& game, uint64_t state_id) {
    const auto& db = drogon::app().getDbClient();
    uint64_t game_id = game.getPrimaryKey();
    uint64_t old_state_id = *game.getStateid();
    db->execSqlSync("UPDATE history SET next = ? WHERE gameId = ? AND stateId = ?",
                    state_id, game_id, old_state_id);
    db->execSqlSync("UPDATE games SET stateId = ?, newStateId = NULL WHERE gameId = ?",
                    state_id, game_id);

    if (state_id == old_state_id) {
        db->execSqlSync("INSERT INTO history (gameId, stateId) VALUES (?, ?)",
                        game_id, state_id);
    } else {
        db->execSqlSync("INSERT INTO history (gameId, stateId, prev) VALUES (?, ?, ?)",
                        game_id, state_id, old_state_id);
    }
}
}
}
