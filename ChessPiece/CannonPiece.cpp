#include "CannonPiece.hpp"

CannonPiece::CannonPiece(std::string img, Engine::Point position, PieceColor country, int score)
    : ChessPiece(img, position, country, score) {
}

bool CannonPiece::IsValidMove(int row, int col,int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard){
    // The piece there is of the same country:
    if (Chessboard[row][col].first * Chessboard[next_row][next_col].first > 0){
        return false;
    }

    // Like ChariotPiece, it should be either of the same row or of the same column.
    if (!(row == next_row || col == next_col)) {
        return false;
    
    } else {
        int min, max, count = 0;
        if (row == next_row) { // Of the same row
            min = (col < next_col) ? col : next_col;
            max = (col > next_col) ? col : next_col;
            for (int i = min + 1; i <= max - 1; i++) {
                auto temp = Chessboard[row][i].second;
                if (temp != nullptr) count++;
            }
        } else { // Of the same column
            min = row < next_row ? row : next_row;
            max = row > next_row ? row : next_row;
            for (int i = min + 1; i <= max - 1; i++){
                auto temp = Chessboard[i][col].second;
                if (temp != nullptr) count++;
            }
        }
        auto temp = Chessboard[next_row][next_col].second;
        if ((count == 0 && temp == nullptr) /* With no piece in between && no piece at the destination */\
            || (count == 1 && temp != nullptr) /* With ONE piece in between && one piece at the destination */)
            return true;
        else
            return false;
    }
}