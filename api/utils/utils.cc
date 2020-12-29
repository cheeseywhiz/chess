#include "utils.h"

HttpResponsePtr to_error(HttpStatusCode status_code, const std::string& reason) {
    static const std::map<HttpStatusCode, std::string> messages = {
        { HttpStatusCode::k400BadRequest, "Bad Request" },
        { HttpStatusCode::k401Unauthorized, "Unauthorized" },
        { HttpStatusCode::k403Forbidden, "Forbidden" },
        { HttpStatusCode::k404NotFound, "Not Found" },
        { HttpStatusCode::k409Conflict, "Conflict" },
    };
    Json::Value res;
    res["status"] = status_code;
    res["message"] = messages.at(status_code);
    res["reason"] = reason;
    auto res2 = drogon::toResponse(res);
    res2->setStatusCode(status_code);
    return res2;
}

uint64_t last_insert_rowid(void) {
    const auto& db = drogon::app().getDbClient();
    const auto& rows = db->execSqlSync("SELECT last_insert_rowid() AS rowid");
    assert(rows.size() == 1);
    return rows[0]["rowid"].as<uint64_t>();
}
