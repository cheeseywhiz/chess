#pragma once
#include <drogon/HttpFilter.h>
using drogon::HttpFilter, drogon::HttpRequestPtr, drogon::FilterCallback, drogon::FilterChainCallback;


class RequireJson : public HttpFilter<RequireJson> {
    public:
        RequireJson() = default;
        virtual void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
};

