#include "PawnPiece.hpp"

PawnPiece::PawnPiece(std::string img, Engine::Point position, PieceColor color, int score)
    : ChessPiece(img, position, color, score) {
}

bool PawnPiece::IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState){
    if(ChessboardState[y][x].first*ChessboardState[next_y][next_x].first>0){
        return false;
    }

    
}