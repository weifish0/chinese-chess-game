#ifndef CHARIOTPIECE_HPP
#define CHARIOTPIECE_HPP

#include "ChessPiece.hpp"

class ChariotPiece final : public ChessPiece {
public:
    ChariotPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece* enemy_piece);
};

#endif // CHARIOTPIECE_HPP
