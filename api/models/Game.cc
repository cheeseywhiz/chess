#include <cassert>
#include "Game.h"
#include <drogon/HttpAppFramework.h>

namespace drogon_model {
namespace sqlite3 {
Game::Ptr Game::lookup_game(int game_id) {
    const auto& db = drogon::app().getDbClient();
    const auto& rows = db->execSqlSync(sqlForFindingByPrimaryKey(), game_id);
    // not found?
    if (rows.size() != 1)
        return nullptr;
    return std::make_shared<Game>(rows[0]);
}

int Game::last_insert_game_id(void) {
    const auto& db = drogon::app().getDbClient();
    const auto& rows = db->execSqlSync("SELECT last_insert_rowid() AS game_id");
    assert(rows.size() == 1);
    return rows[0]["game_id"].as<int>();
}

Game Game::last_insert_game(void) {
    int game_id = last_insert_game_id();
    Ptr game = lookup_game(game_id);
    assert(game);
    return *game;
}
}
}
