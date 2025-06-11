#include "PawnPiece.hpp"

PawnPiece::PawnPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}