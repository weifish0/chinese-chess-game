#ifndef HORSEPIECE_HPP
#define HORSEPIECE_HPP

#include "ChessPiece.hpp"

class HorsePiece final : public ChessPiece {
public:
    HorsePiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece* enemy_piece);
};

#endif // HORSEPIECE_HPP
