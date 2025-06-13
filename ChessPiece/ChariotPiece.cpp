#include "ChariotPiece.hpp"

ChariotPiece::ChariotPiece(std::string img, Engine::Point position, PieceColor country, int score)
    : ChessPiece(img, position, country, score) {
}

bool ChariotPiece::IsValidMove(int row,int col,int next_row,int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard){
    // If the piece is of the same country
    if (Chessboard[row][col].first * Chessboard[next_row][next_col].first > 0) {
        return false;
    }

    // Either of the same column or of the same row
    int min, max, count = 0;
    if (row == next_row){
        min = col > next_col ? next_col : col;
        max = col > next_col ? col : next_col;
        for (int i = min + 1; i <= max - 1; i++) {
            if (Chessboard[next_row][i].second) {
                return false;
            }
        }
        return true;

    } else if (col == next_col) {
        min = row<next_row ? row : next_row;
        max = row>next_row ? row : next_row;

        for (int j = min + 1; j < max; j++) {
            if (Chessboard[j][col].second) {
                return false;
            }
        }
        return true;
    }
    // neither 'of the same row' nor 'of the same column'.
    return false;
}