#pragma once
#include <drogon/HttpFilter.h>
using drogon::HttpFilter, drogon::HttpRequestPtr, drogon::FilterCallback,
      drogon::FilterChainCallback;

class RequireAuth : public HttpFilter<RequireAuth> {
    public:
        RequireAuth() = default;
        virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                              FilterChainCallback&& fccb) override;
};

