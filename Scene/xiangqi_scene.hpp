#ifndef XIANGQISCENE_HPP
#define XIANGQISCENE_HPP
#include <allegro5/allegro_audio.h>
#include <map>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/Collider.hpp"
#include "Engine/IScene.hpp"
#include "ChessPiece/ChessPiece.hpp"

// Engine::Collider collider; // Collider in XiangqiScene.
//                            // Due to the constructor of the collider, it is deemed as a non-static, const variable.
//                            // Hence not allowed to put inside XiangqiScene.

class XiangqiScene final : public Engine::IScene {
private:
    const int ChessboardHeight = 10;
    const int ChessboardWidth = 9;
    
public:
    /* PROPERTIES */
    // Groups (can be seen as layers.)
    Engine::Group *ChessboardGroup; // Chessboard. 棋盤。
    Engine::Group *PieceGroup;      // Pieces on chessboard. 場上的棋子們。
    Engine::Group *UIGroup;         // UI interface: Surrender flag, Back-to-main btn, Preview of chess pieces. UI 介面（投降、回主畫面、預覽）。

    std::vector<std::vector<int>> ChessboardState; // Note down the chess on the chessboard.
    PieceColor round;
    ChessPiece *preview;                           // Preview of the selected chess piece.
    ChessPiece *selectedPiece;                     // Selected Piece.
    std::map<Engine::Point, ChessPiece*> PieceMap; // Keep track of each ChessPiece (using Engine::Point(Palace Point) as the key).

    bool ExpertMode = false;
    bool SelectFlag;

    /* METHODS */
    explicit XiangqiScene() = default;
    void Initialize() override;
    void ReadChessboard();
    // void Update();
    // void SelectedPiecePathExpand();
    void OnMouseDown(int button, int mx, int my);
    void OnMouseMove(int mx, int my);
    void OnMouseUp(int button, int mx, int my);
};
#endif   // XIANGQISCENE_HPP
