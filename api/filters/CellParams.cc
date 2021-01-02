#include "CellParams.h"
#include "utils.h"
#include "chess.h"

void CellParams::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                          FilterChainCallback&& fccb) {
    size_t row;

    try {
        row = std::stoi(req->getParameter("row"));
    } catch (const std::invalid_argument& err) {
        return fcb(to_error(HttpStatusCode::k400BadRequest, "bad row"));
    }

    if (row >= Chess::BOARD_HEIGHT)
        return fcb(to_error(HttpStatusCode::k400BadRequest, "row out of range"));

    size_t col;

    try {
        col = std::stoi(req->getParameter("col"));
    } catch (const std::invalid_argument& err) {
        return fcb(to_error(HttpStatusCode::k400BadRequest, "bad col"));
    }

    if (col >= Chess::BOARD_WIDTH)
        return fcb(to_error(HttpStatusCode::k400BadRequest, "col out of range"));
    fccb();
}
