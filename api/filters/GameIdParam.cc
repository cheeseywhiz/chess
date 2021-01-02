#include "GameIdParam.h"
#include "models/Game.h"
#include "utils.h"
using drogon_model::sqlite3::Game;

// RequireAuth
void GameIdParam::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                          FilterChainCallback&& fccb) {
    const auto& game_id_in = req->getParameter("gameId");
    uint64_t game_id;

    try {
        game_id = std::stoi(game_id_in);
    } catch (const std::invalid_argument& err) {
        return fcb(to_error(HttpStatusCode::k400BadRequest, "bad game_id"));
    }

    const auto& game = Game::lookup_game(game_id);
    if (!game)
        return fcb(to_error(HttpStatusCode::k404NotFound, "Unknown game_id"));
    const auto& username = req->session()->get<std::string>("username");
    if (username != *game->getWhite() && username != *game->getBlack())
        return fcb(to_error(HttpStatusCode::k403Forbidden, "game is not owned by username"));
    fccb();
}
