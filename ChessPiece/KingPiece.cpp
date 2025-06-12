#include "KingPiece.hpp"

KingPiece::KingPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}

bool KingPiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState) {
    
    // Of the same color
    if (ChessboardState[row][col].first * ChessboardState[next_row][next_col].first > 0) {
        return false;
    }

    // Within one step
    if (abs(row - next_row) + abs(col - next_col) == 1) {
        if (IsInPalace(next_row, next_col)) return true;
        else return false;

    } else if (col == next_col && ChessboardState[next_row][next_col].first == -color * KING) { // The king of another country (color)
        int min_r, max_r;
        min_r = row < next_row ? row : next_row;
        max_r = row > next_row ? row : next_row;
        for (int r = min_r + 1; r < max_r; r++) {
            if (ChessboardState[r][next_col].second != nullptr) { // There IS a piece in between.
                return false;
            }
        }
        return true;
    }

    // Default
    return false;
}