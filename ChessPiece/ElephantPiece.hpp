#ifndef ELEPHANTPIECE_HPP
#define ELEPHANTPIECE_HPP

#include <iostream>
#include "ChessPiece/ChessPiece.hpp"

class ElephantPiece final : public ChessPiece {
public:
    ChessPiece* Clone() const override {
        std::cout << "[LOG] Elfnt Clone!" << std::endl;
        return new ElephantPiece(*this);
    }
    explicit ElephantPiece(std::string img, Engine::Point position, PieceColor color, int score);
    bool IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState);
    // void OnAnimation(ChessPiece *enemy_chess);
};

#endif // ELEPHANTPIECE_HPP