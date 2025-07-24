#include "../include/handlers/AiGameStartHandler.h"

void AiGameStartHandler::handle(const http::HttpRequest &req, http::HttpResponse *resp) 
{
    auto session = server_->getSessionManager()->getSession(req, resp);
    if (session->getValue("isLoggedIn") != "true")
    {
        // 用户未登录，返回未授权错误
        json errorResp;
        errorResp["status"] = "error";
        errorResp["message"] = "Unauthorized";
        std::string errorBody = errorResp.dump(4);

        server_->packageResp(req.getVersion(), http::HttpResponse::k401Unauthorized,
                             "Unauthorized", true, "application/json", errorBody.size(),
                             errorBody, resp);
        return;
    }

    int userId = std::stoi(session->getValue("userId"));

    // 创建新的 AI 游戏实例（或重置旧的）
    {
        std::lock_guard<std::mutex> lock(server_->mutexForAiGames_);
        server_->aiGames_.erase(userId);  // 无论有无都删除一次
        server_->aiGames_[userId] = std::make_shared<AiGame>(userId);
    }

    // 构建 JSON 响应给客户端
    json response;
    response["status"] = "ok";
    response["message"] = "AI game started";
    response["userId"] = userId;

    std::string responseBody = response.dump(4);

    resp->setStatusLine(req.getVersion(), http::HttpResponse::k200Ok, "OK");
    resp->setCloseConnection(false);
    resp->setContentType("application/json");
    resp->setContentLength(responseBody.size());
    resp->setBody(responseBody);
}
