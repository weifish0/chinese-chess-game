#include "ChessPiece/GuardPiece.hpp"

GuardPiece::GuardPiece(std::string img, Engine::Point position, PieceColor country, int score)
    : ChessPiece(img, position, country, score) {
}


bool GuardPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard){
    if (Chessboard[y][x].first * Chessboard[next_y][next_x].first > 0) {
        return false;
    }

    if (abs(next_y-y) == 1 && abs(next_x-x) == 1) {
        if (Chessboard[y][x].first < 0) {
            if (!(next_y >= 7 && next_y <= 9) || !(next_x >= 3 && next_x <= 5))
                return false;
            else
                return true;
        }
        else if(Chessboard[y][x].first>0){
            if(!(next_y>=0&&next_y<=2)||!(next_x>=3&&next_x<=5))
                return false;
            else 
                return true;
        }
    }
    return false;
}
