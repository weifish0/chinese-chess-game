#ifndef KINGPIECE_HPP
#define KINGPIECE_HPP

#include <iostream>
#include <utility>
#include <set>
#include <unordered_set>

#include "ChessPiece.hpp"

#include "Engine/GameEngine.hpp"

namespace Engine {
    struct Point;
}

class KingPiece final : public ChessPiece {
private:
    /* 
     * Since KingPiece is confined to the "palace",
     * we'll just keep track of the "relative" position of the piece in the palace.
     */
    std::pair<int, int> currentPalcacePosition;
    std::set<std::pair<int, int>> PossiblePalacePosition;
public:
    explicit KingPiece(std::string img, Engine::Point position, PieceColor color, int score);
    ChessPiece* Clone() const override {
        std::cout << "[LOG] King Clone!" << std::endl;
        return new KingPiece(*this);
    }
    
   bool KingPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState);
    // void OnAnimation(ChessPiece *enemy_chess);
};

#endif // KINGPIECE_HPP