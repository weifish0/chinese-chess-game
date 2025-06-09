#ifndef PAWNPIECE_HPP
#define PAWNPIECE_HPP

#include "ChessPiece.hpp"

class PawnPiece final : public ChessPiece {
public:
    PawnPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece *enemy_piece);
};

#endif // PAWNPIECE_HPP