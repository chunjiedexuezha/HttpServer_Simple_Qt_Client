#pragma once
#include "../../../../HttpServer/include/router/RouterHandler.h"
#include "../XuanServer.h"

class EntryHandler : public http::router::RouterHandler 
{
public:
    explicit EntryHandler(XuanServer* server) : server_(server) {}

    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;

private:
    XuanServer* server_;
};