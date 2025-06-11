#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

#include <iostream>
#include <set>
#include <vector>

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"

enum PieceType { // [RED]     [BLACK]
    NONE,
    PAWN,        // 兵 bing   卒 zu
    CANNON,      // 炮 pao    包 pao
    CHARIOT,     // 俥 ju     車 ju
    HORSE,       // 傌 ma     馬 ma
    ELFNT,       // 相 xiang  象 xiang
    GUARD,       // 仕 shi    士 shi
    KING,        // 帥 shuai  將 jiang    
};
enum PieceColor {
    RED = -1, BLACK = 1
};

const float blockSize = (float) 850 / (float) 11; // Block size on the chessboard.

class ChessPiece : public Engine::Sprite {
protected:
    PieceColor color;
    int score;

public:
    /* METHOD */
    virtual ChessPiece* Clone() const {
        std::cout << "[LOG] ChessPiece Clone!" << std::endl;
        return new ChessPiece(*this); // Default copy behavior (@chatgpt)
    }
    virtual ~ChessPiece() {};  // Always make base destructors virtual - safe default (@chatgpt)
    ChessPiece(std::string img, Engine::Point position, PieceColor color, int score);
    virtual bool IsValidMove(int y,int x,int next_y,int next_x, std::vector<std::vector<std::pair<int,ChessPiece*>>> ChessboardState);
    virtual void OnAnimation(ChessPiece *enemy_piece);
};

#endif // CHESSPIECE_HPP