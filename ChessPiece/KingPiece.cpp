#include "KingPiece.hpp"

KingPiece::KingPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {

    if (this->color == BLACK) {
        // All the possible palace position for black KingPiece.
        this->PossiblePalacePosition = {
            {3, 0}, {4, 0}, {5, 0},
            {3, 1}, {4, 1}, {5, 1},
            {3, 2}, {4, 2}, {5, 2}
        };
        // Current palace position for black KingPiece.
        this->currentPalcacePosition = {4, 0};

    } else {
        // All the possible palace position for red KingPiece.
        this->PossiblePalacePosition = {
            {3, 7}, {4, 7}, {5, 7},
            {3, 8}, {4, 8}, {5, 8},
            {3, 9}, {4, 9}, {5, 9}
        };
        // Current palace position for red KingPiece.
        this->currentPalcacePosition = {4, 9};
    }
}

bool KingPiece::IsValidMove(Engine::Point nextPos, std::vector<std::vector<int>> ChessboardState) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;
    
    // Conversion of nextPos to Palace Point format: 
    // >>> PalacePoint {j, i} 
    //     ≡ Engine::Point (blockSize * (j-4) + halfW, blockSize * (i-4.5) + halfH)
    int _x = (nextPos.x - halfW) / blockSize + 4;                   // Palace x position of `nextPos`.
    int _y = (float) (nextPos.y - halfH) / (float) blockSize + 4.5; // Palace y position of `nextPos`.
    
    // Check if the nextPos is already on the outside of the palace.
    if (PossiblePalacePosition.find({_x, _y}) == PossiblePalacePosition.cend())
        return false;

    // Check if it is withing 1 palace distance (horizontally or vertically).
    if (std::abs(_x - currentPalcacePosition.first) + std::abs(_y - currentPalcacePosition.second) != 1)
        return false;

    // Check if, in case of chess piece encounter, the chess piece is of the same color,
    // If so, the move should be INVALID;
    // Or else, the move should be considered as "EATING" an enemy chess piece.
    if (ChessboardState[_x][_y] * color > 0) // Of the same color
        return false;

    return true;
}