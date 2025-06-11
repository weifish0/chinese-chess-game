#ifndef GUARDPIECE_HPP
#define GUARDPIECE_HPP

#include <iostream>
#include "ChessPiece/ChessPiece.hpp"

namespace Engine {
    struct Point;
}

class GuardPiece final : public ChessPiece {
public:
    ChessPiece* Clone() const override {
        std::cout << "[LOG] Guard Clone!" << std::endl;
        return new GuardPiece(*this);
    }
    explicit GuardPiece(std::string img, Engine::Point position, PieceColor color, int score);
    bool IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState);
    
    // void OnAnimation(ChessPiece *enemy_chess);
};

#endif // GUARDPIECE_HPP