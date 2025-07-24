#include "../include/handlers/PvpStartHandler.h"
#include <iostream>
#include <string>
#include <mutex>

#include <algorithm>



void PvpStartHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) {
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


    auto& games = server_->getPvpGames();
    auto& pending = server_->getMatchedPlayers();
    std::lock_guard<std::mutex> lock(server_->getPvpGameMutex());

    // 如果已匹配成功但尚未取出，直接返回对局信息
    if (pending.count(userId)) {
        
        int gameId = pending[userId];
        auto game = games[gameId];
        pending.erase(userId);
        json respJson = {
            {"status", "ok"},
            {"message", "match_success"},
            {"game_id", gameId},
            {"role", game->getRole(userId) == PieceType::BLACK ? "black" : "white"}
        };
        std::string body = respJson.dump(4);

        server_->packageResp(req.getVersion(), http::HttpResponse::k200Ok,
                             "OK", false, "application/json", body.size(), body, resp);
        return;
    }

    
    auto& waitingQueue = server_->getPvpMatchQueue();
    // 检查队列中是否有人等待配对
    if (waitingQueue.size()>1||(waitingQueue.size()==1&&waitingQueue.front() != userId)) {
        // std::cout<<"进入了这个条件语句";
        if(waitingQueue.front() == userId){
            int tempID = waitingQueue.front();
            waitingQueue.pop();
            waitingQueue.push(tempID);

        }
        int opponentId = waitingQueue.front();
        waitingQueue.pop();

        int newGameId = userId * 10000 + opponentId;  // 简单的唯一 ID 生成
        auto newGame = std::make_shared<PVPGame>(userId, opponentId,newGameId);
        games[newGameId] = newGame;

        pending[userId] = newGameId;
        pending[opponentId] = newGameId;

        json respJson = {
            {"status", "ok"},
            {"message", "match_success"},
            {"game_id", newGameId},
            {"role", newGame->getRole(userId) == PieceType::BLACK ? "black" : "white"}
        };
        std::string body = respJson.dump(4);

        server_->packageResp(req.getVersion(), http::HttpResponse::k200Ok,
                             "OK", false, "application/json", body.size(), body, resp);
    } else {
        if(waitingQueue.empty()){
            // 没有等待者，加入匹配队列
        waitingQueue.push(userId);
        }
        
        json respJson = {
            {"status", "ok"},
            {"message", "waiting_for_opponent"}
        };
        std::string body = respJson.dump(4);

        server_->packageResp(req.getVersion(), http::HttpResponse::k200Ok,
                             "OK", false, "application/json", body.size(), body, resp);
    }
}