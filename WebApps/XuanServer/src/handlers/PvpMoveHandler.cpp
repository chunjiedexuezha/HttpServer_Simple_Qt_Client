#include "../include/handlers/PvpMoveHandler.h"
#include <iostream>
#include <string>
#include <mutex>

#include <algorithm>



void PvpMoveHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) {
    auto session = server_->getSessionManager()->getSession(req, resp);
    if (session->getValue("isLoggedIn") != "true") {
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
    // 解析请求体
        json request = json::parse(req.getBody());
        int x = request["x"];
        int y = request["y"];
        int gameId = request["game_id"];

    auto& games = server_->getPvpGames();
    auto it = games.find(gameId);
    if (it == games.end()) {
        json errorResp;
        errorResp["status"] = "error";
        errorResp["message"] = "Game not found";

        std::string errorBody = errorResp.dump(4);
        server_->packageResp(req.getVersion(), http::HttpResponse::k401Unauthorized,
                             "Unauthorized", true, "application/json", errorBody.size(),
                             errorBody, resp);
        return;
    }
    auto game = it->second;
    // 执行落子逻辑
    if (!game->makeMove(userId, x, y)) {
        json errorResp;
        errorResp["status"] = "error";
        errorResp["message"] = "Invalid move";

        std::string errorBody = errorResp.dump(4);
        server_->packageResp(req.getVersion(), http::HttpResponse::k401Unauthorized,
                             "Unauthorized", true, "application/json", errorBody.size(),
                             errorBody, resp);
        return;
    }
    

    // 生成响应
    json resJson = {
        {"status", "ok"},
        {"message", "move_saved"}
    };
    std::string body = resJson.dump(4);
    server_->packageResp(req.getVersion(), http::HttpResponse::k200Ok,
                         "OK", false, "application/json", body.size(), body, resp);

    
    
}