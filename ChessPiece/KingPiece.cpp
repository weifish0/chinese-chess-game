#include "KingPiece.hpp"

KingPiece::KingPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}

bool KingPiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState) {
    
    // Of the same color
    if (ChessboardState[row][col].first * ChessboardState[next_row][next_col].first > 0) {
        return false;
    }

    // Within one step
    if (abs(row - next_row) + abs(col - next_col) == 1) {
        if (color == RED) {
            std::cout << "[DEBUGGER] KingPiece::IsValidMove 001" << std::endl;
            // Should be inside RED palace
            if (!(next_row >= 7 && next_row <= 9) || !(next_col >= 3 && next_col <= 5))
                return false;
            else
                return true;

        } else if (color == BLACK) {
            std::cout << "[DEBUGGER] KingPiece::IsValidMove 002" << std::endl;
            // Should be inside BLACK palace
            if (!(next_row >= 0 && next_row <= 2) || !(next_col >= 3 && next_col <= 5))
                return false;
            else 
                return true;
        }

    } else if (col == next_col && ChessboardState[next_row][next_col].first == -color * KING) { // The king of another country (color)
        std::cout << "[DEBUGGER] KingPiece::IsValidMove 003" << std::endl;
        int min_r, max_r;
        min_r = row < next_row ? row : next_row;
        max_r = row > next_row ? row : next_row;
        for (int r = min_r + 1; r < max_r; r++) {
            if (ChessboardState[r][next_col].second != nullptr) { // There IS a piece in between.
                std::cout << "[DEBUGGER] KingPiece::IsValidMove 004" << std::endl;
                return false;
            }
        }
        return true;
    }

    // Default
    return false;
}