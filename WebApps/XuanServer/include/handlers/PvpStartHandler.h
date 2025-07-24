#pragma once

#include "../../../HttpServer/include/router/RouterHandler.h"
#include "../XuanServer.h"
#include "../../../HttpServer/include/utils/JsonUtil.h"
#include <memory>


class PvpStartHandler : public http::router::RouterHandler
{
public:
    explicit PvpStartHandler(XuanServer* server) : server_(server) {}

    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    XuanServer* server_;
};