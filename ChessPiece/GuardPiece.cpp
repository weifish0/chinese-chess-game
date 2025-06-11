#include "ChessPiece/GuardPiece.hpp"

GuardPiece::GuardPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}