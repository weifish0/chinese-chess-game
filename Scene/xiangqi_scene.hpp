#ifndef XIANGQISCENE_HPP
#define XIANGQISCENE_HPP
#include <allegro5/allegro_audio.h>
#include <map>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "ChessPiece/ChessPiece.hpp"

class XiangqiScene final : public Engine::IScene {
private:
    const int ChessboardHeight = 10;
    const int ChessboardWidth = 9;
    
public:
    /* PROPERTIES */
    Engine::Group *PieceGroup; // Pieces on chessboard. 場上的棋子們。
    Engine::Group *UIGroup;    // UI interface: Chessboard, surrender flag, back-to-main btn, ... UI 介面
    std::vector<std::vector<int>> ChessboardState; // Note down the chess on the chessboard.

    /* METHODS */
    explicit XiangqiScene() = default;
    void Initialize() override;
    void ReadChessboard();
    // void Update();
    // void OnMouseDown();
    // void OnMouseUp();
};
#endif   // XIANGQISCENE_HPP
