#include "ChariotPiece.hpp"

ChariotPiece::ChariotPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}

bool ChariotPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState){
    if(ChessboardState[y][x].first*ChessboardState[next_y][next_x].first>0){
        return false;
    }
    if(y!=next_y&&x!=next_x){
        return false;
    }
    else
        return true;
    
}