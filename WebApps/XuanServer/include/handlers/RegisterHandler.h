#pragma once
#include "../../../../HttpServer/include/router/RouterHandler.h"
#include "../../../HttpServer/include/utils/MysqlUtil.h"
#include "../XuanServer.h"

class RegisterHandler : public http::router::RouterHandler 
{
public:
    explicit RegisterHandler(XuanServer* server) : server_(server) {}

    void handle(const http::HttpRequest& req, http::HttpResponse* resp) override;
private:
    int insertUser(const std::string& username, const std::string& password);
    bool isUserExist(const std::string& username);
private:
    XuanServer* server_;
    http::MysqlUtil     mysqlUtil_;
};