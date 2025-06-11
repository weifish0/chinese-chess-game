#include "CannonPiece.hpp"

CannonPiece::CannonPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}

bool CannonPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState){
    if(ChessboardState[y][x].first*ChessboardState[next_y][next_x].first>0){
        return false;
    }
    if(y!=next_y&&x!=next_x){
        return false;
    }
    else{
        int min,max,count = 0;
        if(y==next_y){
            min = x<next_x?x:next_x;
            max = x>next_x?x:next_x;
            for(int i=min+1;i<=max-1;i++){
                auto temp = ChessboardState[y][i].second;
                if(temp!=nullptr)
                    count++;
            }
        }
        else{
            min = y<next_y?y:next_y;
            max = y>next_y?y:next_y;
            for(int i=min+1;i<=max-1;i++){
                auto temp = ChessboardState[i][x].second;
                if(temp!=nullptr)
                    count++;
            }
        }
        auto temp = ChessboardState[next_y][next_x].second;
        if(count==0&&temp==nullptr||count==1&&temp!=nullptr)
            return true;
        else
            return false;
    }
}