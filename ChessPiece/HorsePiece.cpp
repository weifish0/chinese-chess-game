#include "HorsePiece.hpp"

HorsePiece::HorsePiece(std::string img, Engine::Point position, PieceColor color, int score) 
    : ChessPiece(img, position, color, score) {
}

bool HorsePiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard){
    // Of the same country
    if (country * Chessboard[next_row][next_col].first > 0) {
        return false;
    }
    
    // Ｉf the next_row is different from row by one.
    // ｏ　ｘ　ｘ　ｘ　ｏ
    // ｘ　ｘ　馬　ｘ　ｘ
    // ｏ　ｘ　ｘ　ｘ　ｏ
    if (abs(row - next_row) == 1) {
        if (col > next_col && abs(col - next_col) == 2 && Chessboard[row][col-1].second == nullptr) {
            return true;

        } else if (col < next_col && abs(col - next_col) == 2 && Chessboard[row][col+1].second == nullptr) {
            return true;
        }
        return false;
    }
    // Else the next_row is different from row by two.
    // ｏ　ｘ　ｏ
    // ｘ　ｘ　ｘ
    // ｘ　馬　ｘ
    // ｘ　ｘ　ｘ
    // ｏ　ｘ　ｏ
    else if (abs(row - next_row) == 2) {

        if (row > next_row && abs(col - next_col) == 1 && Chessboard[row-1][col].second == nullptr){
            return true;

        } else if (row < next_row && abs(col - next_col) == 1 && Chessboard[row+1][col].second == nullptr){
            return true;
        }
        return false;
    }
    
    return false;
}