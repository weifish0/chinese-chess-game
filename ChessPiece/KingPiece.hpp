#ifndef KINGPIECE_HPP
#define KINGPIECE_HPP

#include "ChessPiece.hpp"

namespace Engine {
    struct Point;
}

class KingPiece final : public ChessPiece {
public:
    explicit KingPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    // void OnAnimation(ChessPiece *enemy_chess);
};

#endif // KINGPIECE_HPP