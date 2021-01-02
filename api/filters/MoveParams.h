#pragma once
#include <drogon/HttpFilter.h>
using drogon::HttpFilter, drogon::HttpRequestPtr, drogon::FilterCallback,
      drogon::FilterChainCallback;

class MoveParams : public HttpFilter<MoveParams> {
    public:
        MoveParams() = default;
        virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                              FilterChainCallback&& fccb) override;
};
