#include <cassert>
#include "Game.h"
#include "utils.h"
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

Game Game::create_new_game(const std::string& white, const std::string& black) {
    const auto& db = drogon::app().getDbClient();
    db->execSqlSync("INSERT INTO games(white, black) VALUES (?, ?)", white, black);
    uint64_t game_id = last_insert_rowid();
    const auto& game = lookup_game(game_id);
    assert(game);
    return *game;
}
}
}
