#include <cassert>
#include <drogon/HttpAppFramework.h>
#include "Game.h"
#include "State.h"
#include "History.h"
#include "utils.h"
#include "serialize.h"

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

std::vector<Game> Game::lookup_user_games(const User& user) {
    const auto& db = drogon::app().getDbClient();
    const auto& username = user.getPrimaryKey();
    const auto& rows = db->execSqlSync("SELECT * FROM games WHERE white = ? OR black = ?",
                                       username, username);
    std::vector<Game> games;
    for (const auto& row : rows)
        games.emplace_back(row);
    return games;
}

Game Game::create_new_game(const std::string& white, const std::string& black) {
    uint64_t state_id = State::insert_state(ChessState());
    const auto& db = drogon::app().getDbClient();
    db->execSqlSync("INSERT INTO games(stateId, white, black) VALUES (?, ?, ?)",
                    state_id, white, black);
    uint64_t game_id = last_insert_rowid();
    const auto& game = lookup_game(game_id);
    assert(game);
    History2::push(*game, state_id);
    return *game;
}

uint64_t Game::set_new_state(const Chess::ChessState& chess_state) {
    const auto& db = drogon::app().getDbClient();
    const auto& new_state_id_ptr = getNewstateid();
    uint64_t new_state_id;

    if (new_state_id_ptr) {
        // overwrite
        new_state_id = *new_state_id_ptr;
        State::set_state(new_state_id, chess_state);
    } else {
        // insert new
        new_state_id = State::insert_state(chess_state);
    }

    uint64_t game_id = getPrimaryKey();
    db->execSqlSync("UPDATE games SET newStateId = ? WHERE gameId = ?", new_state_id, game_id);
    return new_state_id;
}

Json::Value Game::serialize(void) const {
    Json::Value json = toJson();
    json.removeMember("stateId");
    const auto& state_id = getStateid();
    assert(state_id);
    const auto& chess_state = State::lookup_state(*state_id);
    assert(chess_state);
    Chess::SerializedState serialized_state(*chess_state);
    json["state"] = serialized_state.to_json(*state_id);
    return json;
}
}
}
