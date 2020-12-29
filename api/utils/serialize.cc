#include <cassert>
#include <sstream>
#include <map>
#include "serialize.h"

std::string serialize_json(const Json::Value& json) {
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    return Json::writeString(builder, json);
}

Json::Value parse_json(const std::string& json_in) {
    Json::CharReaderBuilder builder;
    std::stringstream stream;
    stream << json_in;
    Json::Value json;
    JSONCPP_STRING err;
    bool ok = Json::parseFromStream(builder, stream, &json, &err);
    assert(ok);
    return json;
}

namespace Chess {
using std::map;
using std::string;

SerializedState::SerializedState(
    const string& state_in, const string& player_in, const string& endgame_state_in,
    unsigned n_moves_in, const string& white_captures_in, const string& black_captures_in,
    const string& board_in
)
    : state(state_in), player(player_in), endgame_state(endgame_state_in), n_moves(n_moves_in),
      white_captures(parse_json(white_captures_in)), black_captures(parse_json(black_captures_in)),
      board(parse_json(board_in)) {
}

static const map<State, string> state_to_str = {
    { State::Ready, "Ready" },
    { State::Promotion, "Promotion" },
    { State::Endgame, "Endgame" },
};

static const map<Player, string> player_to_str = {
    { Player::None, "None" },
    { Player::White, "White" },
    { Player::Black, "Black" },
};

static const map<Piece, string> piece_to_str = {
    { Piece::None, "None" },
    { Piece::Pawn, "Pawn" },
    { Piece::Knight, "Knight" },
    { Piece::Bishop, "Bishop" },
    { Piece::Rook, "Rook" },
    { Piece::Queen, "Queen" },
    { Piece::King, "King" },
};

static const map<EndgameState, string> endgame_state_to_str = {
    { EndgameState::None, "None" },
    { EndgameState::Check, "Check" },
    { EndgameState::Checkmate, "Checkmate" },
    { EndgameState::Stalemate, "Stalemate" },
};

static Json::Value captures_to_json(const vector<Piece>& captures);
static Json::Value board_to_json(const BoardT& board);

SerializedState::SerializedState(const ChessState& state_in)
    : state(state_to_str.at(state_in.state)), player(player_to_str.at(state_in.player)),
      endgame_state(endgame_state_to_str.at(state_in.endgame_state)), n_moves(state_in.n_moves),
      white_captures(captures_to_json(state_in.white_captures)),
      black_captures(captures_to_json(state_in.black_captures)),
      board(board_to_json(state_in.board)) {
}

static Json::Value captures_to_json(const vector<Piece>& captures) {
    Json::Value json(Json::ValueType::arrayValue);
    for (const Piece& capture : captures)
        json.append(piece_to_str.at(capture));
    return json;
}

static Json::Value board_to_json(const BoardT& board) {
    Json::Value json;

    for (const auto& row : board) {
        Json::Value row_json;

        for (const auto& cell : row) {
            Json::Value cell_json;
            cell_json.append(piece_to_str.at(cell.piece));
            cell_json.append(player_to_str.at(cell.player));
            cell_json.append(cell.has_moved);
            row_json.append(cell_json);
        }

        json.append(row_json);
    }

    return json;
}

static const map<string, State> str_to_state = {
    { "Ready", State::Ready },
    { "Promotion", State::Promotion },
    { "Endgame", State::Endgame },
};

static const map<string, Player> str_to_player = {
    { "None", Player::None },
    { "White", Player::White },
    { "Black", Player::Black },
};

static const map<string, Piece> str_to_piece = {
    { "None", Piece::None },
    { "Pawn", Piece::Pawn },
    { "Knight", Piece::Knight },
    { "Bishop", Piece::Bishop },
    { "Rook", Piece::Rook },
    { "Queen", Piece::Queen },
    { "King", Piece::King },
};

static const map<string, EndgameState> str_to_endgame_state = {
    { "None", EndgameState::None },
    { "Check", EndgameState::Check },
    { "Checkmate", EndgameState::Checkmate },
    { "Stalemate", EndgameState::Stalemate },
};

static vector<Piece> json_to_captures(const Json::Value& json);
static BoardT json_to_board(const Json::Value& json);

ChessState SerializedState::deserialize(void) const {
    return ChessState(
        json_to_board(board), str_to_state.at(state), str_to_player.at(player),
        str_to_endgame_state.at(endgame_state), n_moves, json_to_captures(white_captures),
        json_to_captures(black_captures)
    );
}

static vector<Piece> json_to_captures(const Json::Value& json) {
    vector<Piece> captures;
    for (const auto& capture : json)
        captures.push_back(str_to_piece.at(capture.asString()));
    return captures;
}

static BoardT json_to_board(const Json::Value& json) {
    BoardT board;

    for (int row = 0; row < BOARD_HEIGHT; ++row) {
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            const auto& cell = json[row][col];
            board[row][col] = Cell(
                str_to_piece.at(cell[0].asString()),
                str_to_player.at(cell[1].asString()),
                cell[2].asBool()
            );
        }
    }

    return board;
}

Json::Value SerializedState::to_json(void) const {
    Json::Value json;
    json["state"] = state;
    json["player"] = player;
    json["endgameState"] = endgame_state;
    json["nMoves"] = n_moves;
    json["whiteCaptures"] = white_captures;
    json["blackCaptures"] = black_captures;
    json["board"] = board;
    return json;
}
} // namespace Chess
