#ifndef CHARIOTPIECE_HPP
#define CHARIOTPIECE_HPP

#include <iostream>
#include "ChessPiece.hpp"

class ChariotPiece final : public ChessPiece {
public:
    ChessPiece* Clone() const override {
        std::cout << "[LOG] Chariot Clone!" << std::endl;
        return new ChariotPiece(*this);
    }
    ChariotPiece(std::string img, Engine::Point position, PieceColor color, int score);
    bool IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState) override;
    // void OnAnimation(ChessPiece* enemy_piece);
};

#endif // CHARIOTPIECE_HPP
