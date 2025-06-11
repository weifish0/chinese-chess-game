#include "KingPiece.hpp"

KingPiece::KingPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {

    if (this->color == BLACK) {
        // All the possible palace position for black KingPiece.
        this->PossiblePalacePosition = {
            {3, 0}, {4, 0}, {5, 0},
            {3, 1}, {4, 1}, {5, 1},
            {3, 2}, {4, 2}, {5, 2}
        };
        // Current palace position for black KingPiece.
        this->currentPalcacePosition = {4, 0};

    } else {
        // All the possible palace position for red KingPiece.
        this->PossiblePalacePosition = {
            {3, 7}, {4, 7}, {5, 7},
            {3, 8}, {4, 8}, {5, 8},
            {3, 9}, {4, 9}, {5, 9}
        };
        // Current palace position for red KingPiece.
        this->currentPalcacePosition = {4, 9};
    }
}

bool KingPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState) {
    if(ChessboardState[y][x].first*ChessboardState[next_y][next_x].first>0){
        return false;
    }

    if(abs(y-next_y)+abs(x-next_x)==1){
        if(ChessboardState[y][x].first<0){
            if(!(next_y>=7&&next_y<=9)||!(next_x>=3&&next_x<=5))
                return false;
            else
                return true;
        }
        else if(ChessboardState[y][x].first>0){
            if(!(next_y>=0&&next_y<=2)||!(next_x>=3&&next_x<=5))
                return false;
            else 
                return true;
        }
    }
    return false;
}