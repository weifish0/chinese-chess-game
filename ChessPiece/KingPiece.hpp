#ifndef KINGPIECE_HPP
#define KINGPIECE_HPP

#include <iostream>
#include <utility>
#include <set>
#include <cmath>
#include <unordered_set>

#include "ChessPiece.hpp"

#include "Engine/GameEngine.hpp"

namespace Engine {
    struct Point;
}

class KingPiece final : public ChessPiece {
private:
public:
    int row, col;
    explicit KingPiece(std::string img, Engine::Point position, PieceColor country, int score);
    ChessPiece* Clone() const override {
        std::cout << "[LOG] King Clone!" << std::endl;
        return new KingPiece(*this);
    }
    
    bool IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard) override;
};

#endif // KINGPIECE_HPP