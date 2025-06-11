#ifndef CANNONPIECE_HPP
#define CANNONPIECE_HPP

#include <iostream>
#include "ChessPiece.hpp"

class CannonPiece final : public ChessPiece {
public:
    ChessPiece* Clone() const override {
        std::cout << "[LOG] King Clone!" << std::endl;
        return new CannonPiece(*this);
    }
    CannonPiece(std::string img, Engine::Point position, PieceColor color, int score);
    bool IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState);
    // void OnAnimation();
};

#endif // CANNONPIECE_HPP