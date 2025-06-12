#ifndef PAWNPIECE_HPP
#define PAWNPIECE_HPP

#include <iostream>
#include "ChessPiece.hpp"

class PawnPiece final : public ChessPiece {
public:
    ChessPiece* Clone() const override{
        std::cout << "[LOG] Pawn Clone!" << std::endl;
        return new PawnPiece(*this);
    }
    PawnPiece(std::string img, Engine::Point position, PieceColor color, int score);
    bool IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState) override;
    // void OnAnimation(ChessPiece *enemy_piece);
    
};

#endif // PAWNPIECE_HPP