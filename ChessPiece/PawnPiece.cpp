#include "PawnPiece.hpp"

PawnPiece::PawnPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}

bool PawnPiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState) {
    // One step away only!
    if (std::abs(next_row - row) + std::abs(next_col - col) != 1)
        return false;
    
    if (color == RED) { // Red pawn
        // No turning back!
        if (next_row > row) 
            return false;
        // If the piece would crossed the river
        if (next_row > 4 && next_col != col) {
            // No left/right turn in their own country.
            return false;
        }

    } else if (color == BLACK) { // Black pawn
        // No turning back!
        if (next_row < row) 
            return false;
        // If the piece would crossed the river
        if (next_row < 5 && next_col != col) {
            // No left/right turn in their own country.
            return false;
        }
    }

    // See if the piece (if there is one) there is of the same color
    if (ChessboardState[row][col].first * ChessboardState[next_row][next_col].first > 0) {
        return false;
    }

    return true;    
}