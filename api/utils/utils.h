#include <drogon/HttpController.h>
using drogon::HttpResponsePtr, drogon::HttpStatusCode, drogon::HttpRequestPtr, drogon::HttpResponsePtr;

using Callback = std::function<void (const HttpResponsePtr&)>;
HttpResponsePtr to_error(HttpStatusCode status_code, const std::string& reason);
