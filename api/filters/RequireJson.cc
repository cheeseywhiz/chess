#include "RequireJson.h"
#include "utils.h"

void RequireJson::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                           FilterChainCallback&& fccb) {
    if (req->getJsonObject())
        return fccb();
    fcb(to_error(HttpStatusCode::k400BadRequest, "bad json request body"));
}
