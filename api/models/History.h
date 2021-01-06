#pragma once
#include "auto/History.h"
#include "Game.h"

namespace drogon_model {
namespace sqlite3 {
class History2 : public History {
    public:
        using History::History;
        using Ptr = std::shared_ptr<History2>;

        static Ptr lookup_history(uint64_t game_id, uint64_t state_id);
        static void push(const Game& game, uint64_t state_id);
};
}
}
