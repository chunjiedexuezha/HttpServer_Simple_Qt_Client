#include "../include/GamePVP.h"

PVPGame::PVPGame(int player1Id, int player2Id, int gameId)
    : player1Id_(player1Id),
      player2Id_(player2Id),
      gameId_(gameId),
      player1Role_(PieceType::BLACK),
      player2Role_(PieceType::WHITE),
      lastPlayer_(PieceType::BLACK),
      lastMove_({-1, -1})
{
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            board_[i][j] = PieceType::NONE;
        }
    }
}

int PVPGame::getId() const {
    return gameId_;
}

PieceType PVPGame::getRole(int userId) const {
    if (userId == player1Id_) return player1Role_;
    if (userId == player2Id_) return player2Role_;
    return PieceType::NONE;
}

bool PVPGame::makeMove(int userId, int x, int y) {
    if (x < 0 || x >= 15 || y < 0 || y >= 15) return false;
    if (board_[x][y] != PieceType::NONE) return false;

    PieceType piece = getRole(userId);
    if (piece != lastPlayer_) return false;
    
    board_[x][y] = piece;
    lastMove_ = {x, y};
    if(lastPlayer_==player1Role_) lastPlayer_=player2Role_;
    else lastPlayer_=player1Role_;

    return true;
}

std::pair<int, int> PVPGame::getLastMove() const {
    return lastMove_;
}

PieceType PVPGame::getLastplayer() const{
    return lastPlayer_;
};