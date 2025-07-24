#include "../include/handlers/MenuHandler.h"

void MenuHandler::handle(const http::HttpRequest &req, http::HttpResponse *resp) 
{
    try
    {
        // 检查用户是否已登录
        auto session = server_->getSessionManager()->getSession(req, resp);
        LOG_INFO << "session->getValue(\"isLoggedIn\") = " << session->getValue("isLoggedIn");

        if (session->getValue("isLoggedIn") != "true")
        {
            // 用户未登录，返回未授权错误
            json errorResp;
            errorResp["status"] = "error";
            errorResp["message"] = "Unauthorized";
            std::string errorBody = errorResp.dump(4);

            server_->packageResp(
                req.getVersion(),
                http::HttpResponse::k401Unauthorized,
                "Unauthorized",
                true,
                "application/json",
                errorBody.size(),
                errorBody,
                resp
            );
            return;
        }

        // 获取用户信息
        int userId = std::stoi(session->getValue("userId"));
        std::string username = session->getValue("username");

        // 构造 JSON 响应
        json response;
        response["status"] = "ok";
        response["userId"] = userId;
        response["username"] = username;

        std::string responseBody = response.dump(4);
        resp->setStatusLine(req.getVersion(), http::HttpResponse::k200Ok, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(responseBody.size());
        resp->setBody(responseBody);
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersion(), http::HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
    }
}
