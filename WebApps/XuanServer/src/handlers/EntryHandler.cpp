#include "../include/handlers/EntryHandler.h"

void EntryHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp)
{
   
    json result;
    result["status"] = "ok";
    result["message"] = "连接成功";  // 或 "Connection successful"
    std::string body = result.dump(4);  // 美化输出，可换成 result.dump() 简化

    // 设置响应信息
    resp->setStatusLine(req.getVersion(), http::HttpResponse::k200Ok, "OK");
    resp->setCloseConnection(false);
    resp->setContentType("application/json");
    resp->setContentLength(body.size());
    resp->setBody(body);
}
