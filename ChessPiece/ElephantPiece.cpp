#include "ChessPiece/ElephantPiece.hpp"
#include <cmath>
ElephantPiece::ElephantPiece(std::string img, Engine::Point position, PieceColor color, int score) 
    : ChessPiece(img, position, color, score) {
}

bool ElephantPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState){
    if(ChessboardState[y][x].first*ChessboardState[next_y][next_x].first>0){
        return false;
    }
    
    if(abs(y-next_y)==2&&abs(x-next_x)==2){
        int check_y = (y+next_y)/2;
        int check_x = (x+next_x)/2;
        if(ChessboardState[check_y][check_x].second==nullptr)
            return true;
        else   
            return false;
    }
    else
        return false;
    
}