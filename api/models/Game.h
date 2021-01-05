#pragma once
#include "auto/Games.h"
#include "chess.h"

namespace drogon_model {
namespace sqlite3 {
class Game : public Games {
    public:
        using Games::Games;
        using Ptr = std::shared_ptr<Game>;

        // get Game by primary id
        static Ptr lookup_game(uint64_t game_id);
        static Game create_new_game(const std::string& white, const std::string& black);
        uint64_t set_new_state(const Chess::ChessState& chess_state);
};
}
}
