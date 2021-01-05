#pragma once
#include "auto/History.h"

namespace drogon_model {
namespace sqlite3 {
class History2 : public History {
    public:
        using History::History;

        static void push(uint64_t game_id, uint64_t state_id);
};
}
}
