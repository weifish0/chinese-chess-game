#include "KingPiece.hpp"

KingPiece::KingPiece(std::string img, Engine::Point position, PieceColor country, int score)
    : ChessPiece(img, position, country, score) {
}

bool KingPiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard) {
    
    // Of the same country
    if (Chessboard[row][col].first * Chessboard[next_row][next_col].first > 0) {
        return false;
    }

    if (abs(row - next_row) + abs(col - next_col) == 1) { // Within one step
        if (IsInPalace(next_row, next_col)) return true;
        else return false;

    } else if (col == next_col && abs(Chessboard[next_row][next_col].first) == WANG) { // The king of another country sharing the same column.
        int min_r, max_r;
        min_r = row < next_row ? row : next_row;
        max_r = row > next_row ? row : next_row;
        for (int r = min_r + 1; r < max_r; r++) {
            if (Chessboard[r][col].second != nullptr) { // There IS a piece in between.
                return 0; // No "Flying General".
            }
        }
        return true; // General does "fly".
    }
    // Default
    return false;
}