#ifndef XIANGQISCENE_HPP
#define XIANGQISCENE_HPP
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <map>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <deque>

#include "ChessPiece/ChessPiece.hpp"

#include "Engine/AudioHelper.hpp"
#include "Engine/Collider.hpp"
#include "Engine/IScene.hpp"

#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

using RowCol = std::pair<int, int>;

#define STATES(row, col) (Chessboard[row][col].first)
#define PIECES(row, col) (Chessboard[row][col].second)

// Engine::Collider collider; // Collider in XiangqiScene.
//                            // Due to the constructor of the collider, it is deemed as a non-static, const variable.
//                            // Hence not allowed to put inside XiangqiScene.

enum Action {
    EMPTY, MOVE, EAT, DIE
};
// RECORD (FOR MOVE RETRACTION/REGRET)
class Record {
private:
    int Type; // <PieceColor> * <PieceType>
    ChessPiece *Piece;
    RowCol OldRowCol;
    RowCol NewRowCol; // If the piece "dies", the NewRowCol would be {-1, -1}.
    int Action;
    PieceColor round;

public:
    Record(int Type, ChessPiece *Piece, RowCol Old, RowCol New, int Action, PieceColor round) 
        : Type(Type), Piece(Piece), OldRowCol(Old), NewRowCol(New), Action(Action) {
        
        this->round = round;
    }

    friend std::ostream &operator<<(std::ostream &out, Record &rcd);
    friend class XiangqiScene;
};

class XiangqiScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> dropSample;
    const int ChessboardHeight = 10;
    const int ChessboardWidth = 9;
    
public:
    /* PROPERTIES */
    // Groups (can be seen as layers.)
    Engine::Group *ChessboardGroup; // Chessboard. 棋盤。
    Engine::Group *PieceGroup;      // Pieces on chessboard. 場上的棋子們。
    Engine::Group *UIGroup;         // UI interface: Surrender flag, Back-to-main btn, Preview of chess pieces. UI 介面（投降、回主畫面、預覽）。

    Engine::Image *chessboard;
    ChessPiece *RedKing, *BlackKing;

    Engine::Label *RoundReminder;    // To show the enabled country for the round.
    Engine::Label *RoundWarning1, *RoundWarning2, *RoundWarning3;
    int round_warning_tick;          // Animation tick for RoundWarning.

    Engine::Image *BlackCheckmateWarning;
    Engine::Image *RedCheckmateWarning;
    int checkmate_warning_tick;      // Animation tick for CheckmateWarning.

    Engine::Image *FlyingGeneralImg;
    int flying_general_tick;         // Animation tick for flying general execution. (Positive: black wins/flies; Negative: red wins/flies)
    float general_dist;

    bool WrongPiece = false;

    // Chessboard Storer.
    // >>> Use row-col system (aka. palace position system) as indices to access
    // >>> Content: std::pair<int, ChessPiece*>
    // >>> first: <COLOR> * <PIECETYPE>
    // >>> second: <CHESSPIECE POINTER>
    std::vector<std::vector<std::pair<int, ChessPiece*>>> Chessboard;

    // Row and Column of the piece previously selected by the user.
    // >>> first: <ROW>, second: <COL>
    std::pair<int, int> SelectedRowCol;

    PieceColor Round;
    ChessPiece *preview; // Preview of the selected chess piece.

    bool ExpertMode = false;
    bool SwitchFlag;
    bool SelectFlag;

    std::deque<Record> RegretDeq;
    Engine::ImageButton *RegretBtn;
    Engine::Label *RegretBtnLbl;
    int RegretCount = 3;
    int RegretFlag = 0;
    int regret_tick = 0;
    Engine::Label *RegretWarning;

    Engine::Label *PerpetualPeaceWarning;
    int PerpetualPeaceCount = 0;

    /* METHODS */
    explicit XiangqiScene() = default;
    void Initialize() override;
    void ConstructUI();
    void ReadChessboard();
    void Update(float deltaTime) override;
    void Terminate() override;

    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    bool PieceWithinChessboard(int r, int c);

    /* CHESSBOARD KIT */
    // Moving pieces using this function!
    // ro, co: original row & column
    // rf, cf: final row & column
    void MoveOnChessboard(int ro, int co, int rf, int cf);
    /* MOVE REGRET KIT */
    void InsertRegret(int Type, ChessPiece *Piece, RowCol Old, RowCol New, int Action);
    void RegretOnClick();

    /* DEVELOP KIT */
    int x_to_col(float x);
    int y_to_row(float y);
    float col_to_x(int col);
    float row_to_y(int row);
    void PrintChessboardState();
};
#endif   // XIANGQISCENE_HPP