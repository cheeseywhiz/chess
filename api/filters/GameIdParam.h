#pragma once
#include <drogon/HttpFilter.h>
using drogon::HttpFilter, drogon::HttpRequestPtr, drogon::FilterCallback,
      drogon::FilterChainCallback;

class GameIdParam : public HttpFilter<GameIdParam> {
    public:
        GameIdParam() = default;
        virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                              FilterChainCallback&& fccb) override;
};
