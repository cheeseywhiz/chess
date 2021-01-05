#include <drogon/HttpAppFramework.h>
#include "History.h"

namespace drogon_model {
namespace sqlite3 {
void History2::push(uint64_t game_id, uint64_t state_id) {
    const auto& db = drogon::app().getDbClient();
    db->execSqlSync("UPDATE games SET stateId = ?, newStateId = NULL WHERE gameId = ?", state_id, game_id);
    db->execSqlSync("INSERT INTO history (gameId, stateId) VALUES (?, ?)", game_id, state_id);
}
}
}
