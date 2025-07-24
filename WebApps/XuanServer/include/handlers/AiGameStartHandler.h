#pragma once
#include "../../../../HttpServer/include/router/RouterHandler.h"
#include "../XuanServer.h"

class AiGameStartHandler : public http::router::RouterHandler
{
public:
    explicit AiGameStartHandler(XuanServer* server) : server_(server) {}

    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    XuanServer* server_;
};