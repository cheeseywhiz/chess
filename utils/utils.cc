#include "utils.h"

HttpResponsePtr to_error(HttpStatusCode status_code, const std::string& reason) {
    static const std::map<HttpStatusCode, std::string> messages = {
        { HttpStatusCode::k400BadRequest, "Bad Request" },
        { HttpStatusCode::k401Unauthorized, "Unauthorized" },
        { HttpStatusCode::k404NotFound, "Not Found" },
    };
    Json::Value res;
    res["status"] = status_code;
    res["message"] = messages.at(status_code);
    res["reason"] = reason;
    auto res2 = drogon::toResponse(res);
    res2->setStatusCode(status_code);
    return res2;
}
