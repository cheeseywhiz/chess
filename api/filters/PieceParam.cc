#include "PieceParam.h"
#include "utils.h"
#include "serialize.h"

void PieceParam::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                          FilterChainCallback&& fccb) {
    const auto& piece = req->getParameter("piece");
    if (Chess::str_to_piece.find(piece) == Chess::str_to_piece.end())
        return fcb(to_error(HttpStatusCode::k400BadRequest, "bad piece"));
    fccb();
}
