#pragma once
#include "auto/History.h"
#include "Game.h"

namespace drogon_model {
namespace sqlite3 {
class History2 : public History {
    public:
        using History::History;

        static void push(const Game& game, uint64_t state_id);
};
}
}
