#ifndef HORSEPIECE_HPP
#define HORSEPIECE_HPP

#include <iostream>
#include "ChessPiece.hpp"

class HorsePiece final : public ChessPiece {
public:
    ChessPiece* Clone() const override {
        std::cout << "[LOG] Horse Clone!" << std::endl;
        return new HorsePiece(*this);
    }
    HorsePiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece* enemy_piece);
};

#endif // HORSEPIECE_HPP
