#pragma once

#include <unordered_map>
#include <vector>
#include <utility>
#include <mutex>

enum class PieceType { NONE, BLACK, WHITE };

class PVPGame {
public:
    PVPGame(int player1Id, int player2Id, int gameId);

    int getId() const;
    PieceType getRole(int userId) const;

    bool makeMove(int userId, int x, int y);

    std::pair<int, int> getLastMove() const;
    PieceType getLastplayer() const;

private:
    int gameId_;
    int player1Id_;
    int player2Id_;
    PieceType player1Role_;
    PieceType player2Role_;
    PieceType lastPlayer_;

    PieceType board_[15][15];
    std::pair<int, int> lastMove_;
};