#include "KingPiece.hpp"

KingPiece::KingPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score)
    : ChessPiece(img, position, color, isPreview, score) {
}