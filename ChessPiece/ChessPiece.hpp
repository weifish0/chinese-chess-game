#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

#include <iostream>
#include <set>
#include <vector>
#include <cmath>

#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"

enum PieceType { // [HONG]     [HEI]
    NONE,
    PAWN,        // 兵 bing   卒 zu
    PAO,      // 炮 pao    包 pao
    CHARIOT,     // 俥 ju     車 ju
    MA,       // 傌 ma     馬 ma
    ELFNT,       // 相 xiang  象 xiang
    GUARD,       // 仕 shi    士 shi
    WANG,        // 帥 shuai  將 jiang    
};
enum PieceColor {
    HONG = -1, HEI = 1
};

extern float blockSize; // Block size on the chessboard.

class ChessPiece : public Engine::Sprite {
protected:
    PieceColor color;
    int score;

public:
    /* METHOD */
    virtual ChessPiece* Clone() const;
    virtual ~ChessPiece() {};  // Always make base destructors virtual - safe default (@chatgpt)
    ChessPiece(std::string img, Engine::Point position, PieceColor color, int score);
    virtual bool IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState);
    bool IsCheckmate(std::vector<std::vector<std::pair<int, ChessPiece*>>> &ChessboardState);
    virtual void OnAnimation(ChessPiece *enemy_piece);

    /* DEVELOP KIT */
    bool IsInPalace(int row, int col);
};

#endif // CHESSPIECE_HPP