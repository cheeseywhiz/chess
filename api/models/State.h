#pragma once
#include "auto/States.h"
#include "chess.h"
using Chess::ChessState;

namespace drogon_model {
namespace sqlite3 {
class State : public States {
    public:
        using States::States;

        static ChessState lookup_state(uint64_t state_id);
        static uint64_t insert_state(const ChessState& chess_state);
};
} // namespace sqlite3
} // namespace drogon_model
