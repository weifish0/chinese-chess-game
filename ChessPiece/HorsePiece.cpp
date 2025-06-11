#include "HorsePiece.hpp"

HorsePiece::HorsePiece(std::string img, Engine::Point position, PieceColor color, int score) 
    : ChessPiece(img, position, color, score) {
}

bool HorsePiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState){
    if(ChessboardState[y][x].first*ChessboardState[next_y][next_x].first>0){
        return false;
    }
    
    if(abs(y-next_y)==1){
        if(x>next_x&&abs(x-next_x)==2&&ChessboardState[y][x-1].second==nullptr){
            return true;
        }
        else if(abs(x-next_x)==2&&ChessboardState[y][x+1].second==nullptr){
            return true;
        }
        return false;
    }
    else if(abs(y-next_y)==2){

        if(y>next_y&&abs(y-next_y)==2&&ChessboardState[y-1][x].second==nullptr){
            return true;
        }
        else if(abs(y-next_y)==2&&ChessboardState[y+1][x].second==nullptr){
            return true;
        }
        return false;
    }
    else{
        return false;
    }
}