#include "MoveParams.h"
#include "utils.h"
#include "chess.h"

void MoveParams::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                          FilterChainCallback&& fccb) {
    size_t row2;

    try {
        row2 = std::stoi(req->getParameter("row2"));
    } catch (const std::invalid_argument& err) {
        return fcb(to_error(HttpStatusCode::k400BadRequest, "bad row2"));
    }

    if (row2 >= Chess::BOARD_HEIGHT)
        return fcb(to_error(HttpStatusCode::k400BadRequest, "row2 out of range"));

    size_t col2;

    try {
        col2 = std::stoi(req->getParameter("col2"));
    } catch (const std::invalid_argument& err) {
        return fcb(to_error(HttpStatusCode::k400BadRequest, "bad col2"));
    }

    if (col2 >= Chess::BOARD_WIDTH)
        return fcb(to_error(HttpStatusCode::k400BadRequest, "col2 out of range"));
    fccb();
}
