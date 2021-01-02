#pragma once
#include <drogon/HttpFilter.h>
using drogon::HttpFilter, drogon::HttpRequestPtr, drogon::FilterCallback,
      drogon::FilterChainCallback;

class CellParams : public HttpFilter<CellParams> {
    public:
        CellParams() = default;
        virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,
                              FilterChainCallback&& fccb) override;
};
