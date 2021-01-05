#pragma once
#include <drogon/HttpFilter.h>
using drogon::HttpFilter, drogon::HttpRequestPtr, drogon::FilterCallback,
      drogon::FilterChainCallback;

class PieceParam : public HttpFilter<PieceParam> {
    public:
        PieceParam() = default;
        virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                              FilterChainCallback&& fccb) override;
};
