#ifndef GUARDPIECE_HPP
#define GUARDPIECE_HPP

#include "ChessPiece/ChessPiece.hpp"

namespace Engine {
    struct Point;
}

class GuardPiece final : public ChessPiece {
public:
    explicit GuardPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece *enemy_chess);
};

#endif // GUARDPIECE_HPP