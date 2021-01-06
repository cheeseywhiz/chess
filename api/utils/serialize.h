#pragma once
#include <map>
#include <string>
#include <json/json.h>
#include "chess.h"

std::string serialize_json(const Json::Value&);
Json::Value parse_json(const std::string&);

namespace Chess {
Json::Value serialize_moves(const vector<CellReference>& moves);
Json::Value serialize_castles(Castles);

static const std::map<std::string, Piece> str_to_piece = {
    { "None", Piece::None },
    { "Pawn", Piece::Pawn },
    { "Knight", Piece::Knight },
    { "Bishop", Piece::Bishop },
    { "Rook", Piece::Rook },
    { "Queen", Piece::Queen },
    { "King", Piece::King },
};

struct SerializedState {
    std::string state;
    std::string player;
    std::string endgame_state;
    unsigned n_moves;
    Json::Value white_captures, black_captures;
    Json::Value board;

    SerializedState() = delete;
    SerializedState(
        const std::string& state_in, const std::string& player_in,
        const std::string& endgame_state_in, unsigned n_moves_in,
        const std::string& white_captures_in, const std::string& black_captures_in,
        const std::string& board_in
    );
    explicit SerializedState(const ChessState& chess_state_in);
    ChessState deserialize(void) const;
    Json::Value to_json(uint64_t state_id) const;
};
} // namespace Chess
