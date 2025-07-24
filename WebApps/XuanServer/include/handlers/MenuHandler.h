#pragma once

#include "../../../HttpServer/include/router/RouterHandler.h"
#include "../XuanServer.h"


class MenuHandler : public http::router::RouterHandler
{
public:
    explicit MenuHandler(XuanServer* server) : server_(server) {}

    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;
private:
    XuanServer* server_;
};