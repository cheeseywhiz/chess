#include <drogon/HttpController.h>
using drogon::HttpResponsePtr, drogon::HttpStatusCode, drogon::HttpRequestPtr, drogon::HttpResponsePtr;

using Callback = std::function<void (const HttpResponsePtr&)>;
HttpResponsePtr to_error(HttpStatusCode status_code, const std::string& reason);
uint64_t last_insert_rowid(void);

#define ASSERT_JSON_MEMBER(json, member) \
    do { \
        if (!(json)->isMember((#member))) \
            return callback(to_error(HttpStatusCode::k400BadRequest, "\"" #member "\" not in request body")); \
    } while (0)
