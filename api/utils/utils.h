#include <drogon/HttpController.h>
using drogon::HttpResponsePtr, drogon::HttpStatusCode, drogon::HttpRequestPtr, drogon::HttpResponsePtr;

using Callback = std::function<void (const HttpResponsePtr&)>;
HttpResponsePtr to_error(HttpStatusCode status_code, const std::string& reason);

#define ASSERT_JSON_MEMBER(json, member) \
    do { \
        if (!(json)->isMember((#member))) \
            return callback(to_error(HttpStatusCode::k400BadRequest, "\"" #member "\" not in request body")); \
    } while (0)
