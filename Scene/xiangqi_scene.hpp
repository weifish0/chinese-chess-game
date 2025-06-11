#ifndef XIANGQISCENE_HPP
#define XIANGQISCENE_HPP
#include <allegro5/allegro_audio.h>
#include <map>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/AudioHelper.hpp"
#include "Engine/Collider.hpp"
#include "Engine/IScene.hpp"
#include "ChessPiece/ChessPiece.hpp"
#include "UI/Component/Label.hpp"

// Engine::Collider collider; // Collider in XiangqiScene.
//                            // Due to the constructor of the collider, it is deemed as a non-static, const variable.
//                            // Hence not allowed to put inside XiangqiScene.

class XiangqiScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
    const int ChessboardHeight = 10;
    const int ChessboardWidth = 9;
    
public:
    /* PROPERTIES */
    // Groups (can be seen as layers.)
    Engine::Group *ChessboardGroup; // Chessboard. 棋盤。
    Engine::Group *PieceGroup;      // Pieces on chessboard. 場上的棋子們。
    Engine::Group *UIGroup;         // UI interface: Surrender flag, Back-to-main btn, Preview of chess pieces. UI 介面（投降、回主畫面、預覽）。

    Engine::Image *chessboard;
    bool RedKing, BlackKing;
    Engine::Label *RoundReminder;    // To show the enabled color for the round.
    Engine::Label *RoundWarning1, *RoundWarning2, *RoundWarning3;
    int warning_tick;                // Animation tick for RoundWarning.
    bool WrongPiece = false;

    // ChessboardState Storer.
    // >>> Use row-col system (aka. palace position system) as indices to access
    // >>> Content: std::pair<int, ChessPiece*>
    // >>> first: <COLOR> * <PIECETYPE>
    // >>> second: <CHESSPIECE POINTER>
    std::vector<std::vector<std::pair<int, ChessPiece*>>> ChessboardState;

    // Row and Column of the piece previously selected by the user.
    // >>> first: <ROW>, second: <COL>
    std::pair<int, int> SelectedRowCol;

    PieceColor Round;
    ChessPiece *preview; // Preview of the selected chess piece.

    bool ExpertMode = false;
    bool SwitchFlag;
    bool SelectFlag;

    /* METHODS */
    explicit XiangqiScene() = default;
    void Initialize() override;
    void ReadChessboard();
    void Update(float deltaTime) override;
    void Terminate() override;
    // void SelectedPiecePathExpand();
    void OnMouseDown(int button, int mx, int my);
    void OnMouseMove(int mx, int my);
    void OnMouseUp(int button, int mx, int my);

    /* DEVELOP KIT */
    int x_to_col(float x);
    int y_to_row(float y);
    float col_to_x(int col);
    float row_to_y(int row);
    void PrintChessboardState();
};
#endif   // XIANGQISCENE_HPP
