#include "../include/handlers/PvpPollMoveHandler.h"
#include <iostream>
#include <string>
#include <mutex>

#include <algorithm>


void PvpPollMoveHandler::handle(const http::HttpRequest& req, http::HttpResponse* resp) {
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
    int gameId = std::stoi(req.getQueryParameters("game_id")); 
    


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
    if(game->getRole(userId)!=game->getLastplayer()){
        json resJson;
        resJson["status"] = "ok";
        resJson["message"] = "waiting";

        std::string JsonBody = resJson.dump(4);
        server_->packageResp(req.getVersion(), http::HttpResponse::k200Ok,
                             "OK", false, "application/json", JsonBody.size(),
                             JsonBody, resp);
        return;
    }else{
        auto pove_pos = game->getLastMove();
        json resJson;
        resJson["status"] = "ok";
        resJson["message"] = "opponent_moved";
        resJson["x"] = pove_pos.first;
        resJson["y"] = pove_pos.second;
        


        std::string errorBody = resJson.dump(4);
        server_->packageResp(req.getVersion(), http::HttpResponse::k200Ok,
                             "OK", false, "application/json", errorBody.size(),
                             errorBody, resp);
        return;
    }
    
    
}