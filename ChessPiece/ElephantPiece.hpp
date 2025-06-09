#ifndef ELEPHANTPIECE_HPP
#define ELEPHANTPIECE_HPP

#include "ChessPiece/ChessPiece.hpp"

class ElephantPiece final : public ChessPiece {
public:
    explicit ElephantPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece *enemy_chess);
};

#endif // ELEPHANTPIECE_HPP