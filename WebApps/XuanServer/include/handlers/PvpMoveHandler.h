#pragma once

#include "../../../HttpServer/include/router/RouterHandler.h"
#include "../XuanServer.h"
#include "../../../HttpServer/include/utils/JsonUtil.h"
#include <memory>


class PvpMoveHandler : public http::router::RouterHandler
{
public:
    explicit PvpMoveHandler(XuanServer* server) : server_(server) {}

    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    XuanServer* server_;
};