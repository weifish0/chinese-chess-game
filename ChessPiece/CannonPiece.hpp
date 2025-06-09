#ifndef CANNONPIECE_HPP
#define CANNONPIECE_HPP

#include "ChessPiece.hpp"

class CannonPiece final : public ChessPiece {
public:
    CannonPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation();
};

#endif // CANNONPIECE_HPP