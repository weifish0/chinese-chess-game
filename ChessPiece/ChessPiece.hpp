#ifndef CHESSPIECE_HPP
#define CHESSPIECE_HPP

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
    /* PROPERTY */
    bool isPreview; // For user preview after double-clicking a chess piece.

    /* METHOD */
    explicit ChessPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score);
    virtual bool IsValidMove(Engine::Point nextPos, std::vector<std::vector<int>> ChessboardState);
    virtual void OnAnimation(ChessPiece *enemy_piece);
};

#endif // CHESSPIECE_HPP