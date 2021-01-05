#pragma once
#include <drogon/HttpController.h>
#include "utils.h"
using drogon::HttpRequestPtr, drogon::HttpResponsePtr;
using drogon::Get, drogon::Post;

namespace api {
class game : public drogon::HttpController<game> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(game::new_game, "/new_game", Post, "RequireAuth", "RequireJson");
        METHOD_ADD(game::get_game, "/?gameId={1}", Get, "RequireAuth", "GameIdParam");
        METHOD_ADD(game::get_moves, "/moves/?gameId={1}&row={2}&col={3}", Get, "RequireAuth",
                                    "GameIdParam", "CellParams");
        METHOD_ADD(game::do_move, "/moves/?gameId={1}&row={2}&col={3}&row2={4}&col2={5}", Post,
                                  "RequireAuth", "GameIdParam", "CellParams", "MoveParams");
        METHOD_ADD(game::promote, "/moves/promote/?gameId={1}&piece={2}", Post, "RequireAuth",
                                  "GameIdParam", "PieceParam");
        METHOD_LIST_END

        void new_game(const HttpRequestPtr& req, Callback&& callback);
        void get_game(const HttpRequestPtr& req, Callback&& callback,
                      const std::string& game_id_in);
        void get_moves(
            const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in,
            const std::string& row_in, const std::string& col_in);
        void do_move(
            const HttpRequestPtr& req, Callback&& callback, const std::string& game_id_in,
            const std::string& row1_in, const std::string& col1_in,
            const std::string& row2_in, const std::string& col2_in);
        void promote(
            const HttpRequestPtr& req, Callback&& callbacl, const std::string& game_id_in,
            const std::string& piece_in);
};
}
