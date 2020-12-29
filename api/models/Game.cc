#include <cassert>
#include <drogon/HttpAppFramework.h>
#include "Game.h"
#include "State.h"
#include "utils.h"

namespace drogon_model {
namespace sqlite3 {
Game::Ptr Game::lookup_game(uint64_t game_id) {
    const auto& db = drogon::app().getDbClient();
    const auto& rows = db->execSqlSync(sqlForFindingByPrimaryKey(), game_id);
    // not found?
    if (rows.size() != 1)
        return nullptr;
    return std::make_shared<Game>(rows[0]);
}

Game Game::create_new_game(const std::string& white, const std::string& black) {
    uint64_t state_id = State::insert_state(ChessState());
    const auto& db = drogon::app().getDbClient();
    db->execSqlSync("INSERT INTO games(state_id, white, black) VALUES (?, ?, ?)",
                    state_id, white, black);
    uint64_t game_id = last_insert_rowid();
    const auto& game = lookup_game(game_id);
    game->history_push(state_id);
    assert(game);
    return *game;
}

void Game::history_push(uint64_t state_id) {
    const auto& db = drogon::app().getDbClient();
    uint64_t game_id = getPrimaryKey();
    db->execSqlSync("UPDATE games SET state_id = ? WHERE game_id = ?", state_id, game_id);
    db->execSqlSync("INSERT INTO history (game_id, state_id) VALUES (?, ?)", game_id, state_id);
}
}
}
