#include "RequireAuth.h"
#include "utils.h"

void RequireAuth::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                           FilterChainCallback&& fccb) {
    if (req->session()->find("username"))
        return fccb();
    fcb(to_error(HttpStatusCode::k401Unauthorized, "not logged in"));
}
