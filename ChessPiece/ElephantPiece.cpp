#include "ChessPiece/ElephantPiece.hpp"
#include <cmath>
ElephantPiece::ElephantPiece(std::string img, Engine::Point position, PieceColor country, int score) 
    : ChessPiece(img, position, country, score) {
}

bool ElephantPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard){    
    // If it is of the same country:
    if(Chessboard[y][x].first*Chessboard[next_y][next_x].first>0){
        return false;
    }
    
    // If this piece crosses the river:
    if ((country == HONG && next_y < 5) || (country == HEI && next_y > 4)) {
        return false;
    } 

    if(abs(y-next_y)==2&&abs(x-next_x)==2){
        int check_y = (y+next_y)/2;
        int check_x = (x+next_x)/2;
        if(Chessboard[check_y][check_x].second==nullptr)
            return true;
        else   
            return false;
    }
    else
        return false;
    
}