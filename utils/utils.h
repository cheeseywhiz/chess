#include <drogon/HttpController.h>
using drogon::HttpResponsePtr, drogon::HttpStatusCode, drogon::HttpRequestPtr, drogon::HttpResponsePtr;

HttpResponsePtr to_error(HttpStatusCode status_code, const std::string& reason);
