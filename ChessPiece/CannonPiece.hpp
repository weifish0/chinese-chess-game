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
    // void OnAnimation();
};

#endif // CANNONPIECE_HPP