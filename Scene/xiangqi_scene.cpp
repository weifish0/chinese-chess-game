#include <algorithm>
#include <allegro5/allegro.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "Scene/xiangqi_scene.hpp"

#include "ChessPiece/ChessPiece.hpp"
#include "ChessPiece/CannonPiece.hpp"
#include "ChessPiece/ChariotPiece.hpp"
#include "ChessPiece/ElephantPiece.hpp"
#include "ChessPiece/GuardPiece.hpp"
#include "ChessPiece/HorsePiece.hpp"
#include "ChessPiece/KingPiece.hpp"
#include "ChessPiece/PawnPiece.hpp"

#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"

#include "UI/Component/Image.hpp"

#define STATES(row, col) (ChessboardState[row][col].first)
#define PIECES(row, col) (ChessboardState[row][col].second)

PieceColor winner = RED;

/* DEVELOP KIT */
int XiangqiScene::x_to_col(float x) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    return std::floor((x + 0.5 * blockSize - halfW) / blockSize) + 4;
}
int XiangqiScene::y_to_row(float y) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    return std::floor((y - halfH) / blockSize) + 5;
}
float XiangqiScene::col_to_x(int col) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    return blockSize * (col - 4) + halfW;
}
float XiangqiScene::row_to_y(int row) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    return blockSize * (row - 4.5) + halfH;
}

void XiangqiScene::PrintChessboardState() {
    for (int row = 0; row < ChessboardHeight; row++) {
        for (int col = 0; col < ChessboardWidth; col++) std::cout << std::setw(2) << std::setfill(' ') << ChessboardState[row][col].first << " ";
        std::cout << std::endl;
    }
}

bool XiangqiScene::PieceWithinRange(int r, int c) {
    return (0 <= r && r < ChessboardHeight && 0 <= c && c < ChessboardWidth);
}

/* METHODS */
void XiangqiScene::Initialize() {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;
    
    // Add groups from bottom to top.
    AddNewObject(ChessboardGroup = new Group());
    AddNewObject(PieceGroup = new Group());
    AddNewObject(UIGroup = new Group());

    // Read Chessboard txt file
    ReadChessboard();
    std::cout << "[DEBUGGER] 002" << std::endl;

    ChessboardGroup->AddNewObject(chessboard = new Engine::Image("xiangqi/xiangqi_chessboard_albert.jpg", halfW, halfH, blockSize * 10, blockSize * 11, 0.5, 0.5));
    // Round Reminder (color)
    UIGroup->AddNewObject(new Engine::Label("Round:", "pirulen.ttf", 40, halfW / 4, halfH / 8, 255, 255, 255, 255, 0.5, 0.5));
    UIGroup->AddNewObject(RoundReminder = new Engine::Label("RED", "pirulen.ttf", 40, halfW / 4, halfH / 8 + 40, 255, 255, 255, 255, 0.5, 0.5));
    RoundReminder->Color = al_map_rgba(255, 0, 0, 255); // Current color: RED
    // Round Warning
    UIGroup->AddNewObject(RoundWarning1 = new Engine::Label("Cannot choose", "pirulen.ttf", 30, w - halfW / 4, halfH / 8, 255, 150, 150, 0, 0.5, 0.5));
    UIGroup->AddNewObject(RoundWarning2 = new Engine::Label("BLACK PIECES", "pirulen.ttf", 36, w - halfW / 4, halfH / 8 + 50, 255, 150, 150, 0, 0.5, 0.5));
    UIGroup->AddNewObject(RoundWarning3 = new Engine::Label("in this round!!", "pirulen.ttf", 30, w - halfW / 4, halfH / 8 + 100, 255, 150, 150, 0, 0.5, 0.5));
    std::cout << "[DEBUGGER] 003" << std::endl;

    Round = RED;
    preview = nullptr;
    SelectFlag = false;
    SwitchFlag = false;

    // bgmId = AudioHelper::PlayBGM("play.ogg");
    std::cout << "[DEBUGGER] 004" << std::endl;
}

void XiangqiScene::ReadChessboard() {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;
    ChessboardState.clear();
    ChessboardState = std::vector<std::vector<std::pair<int, ChessPiece*>>>(ChessboardHeight, std::vector<std::pair<int, ChessPiece*>>(ChessboardWidth, {0, nullptr}));
    
    std::string filename = "Resource/xiangqi_chessboard.txt";
    // Read chessboard file.
    char c;
    std::vector<int> mapData;
    std::ifstream fin(filename);
    while (fin >> c) {
        if ('0' <= c && c <= '7') {
            mapData.push_back(c - '0');

        } else if ('8' <= c <= '9') {
            throw std::ios_base::failure("Chessboard data corrupted...");

        } else {
            if (static_cast<int>(mapData.size()) / ChessboardWidth != 0)
                throw std::ios_base::failure("Chessboard data corrupted...");
        }
    }
    fin.close();

    // Validate chessboard file.
    if (static_cast<int>(mapData.size()) != ChessboardHeight * ChessboardWidth)
        throw std::ios_base::failure("Chessboard data corrupted...");
    
    // Store map in 2D array.
    for (int row = 0; row < ChessboardHeight; row++) {
        PieceColor color = (row < ChessboardHeight / 2) ? BLACK : RED;

        for (int col = 0; col < ChessboardWidth; col++) {
            const int num = mapData[row * ChessboardWidth + col];
            STATES(row, col) = color * num; // Red pieces would generally go up (decrease in y), so I set the state of red pieces to negative.
            
            // Construct all the chess pieces.
            // BLACK at the top, RED at the bottom.
            Engine::Point position(col_to_x(col), row_to_y(row)); // Regular form: i for row, j for column!
            Engine::Point palace_position(col, row);                                            // Palace form
            std::string img = "xiangqi/";
            ChessPiece *new_piece = nullptr;

            if (num == KING) {
                if (color == RED) {
                    img += "red_piece_shuai.png";
                    PieceGroup->AddNewObject(new_piece = new KingPiece(img, position, color, KING * 10));
                    RedKing = true;

                } else {
                    img += "black_piece_jiang.png";
                    PieceGroup->AddNewObject(new_piece = new KingPiece(img, position, color, KING * 10));
                    BlackKing = true;
                }

            } else if (num == GUARD) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_shi.png";
                PieceGroup->AddNewObject(new_piece = new GuardPiece(img, position, color, GUARD * 10));

            } else if (num == ELFNT) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_xiang.png";
                PieceGroup->AddNewObject(new_piece = new ElephantPiece(img, position, color, ELFNT * 10));

            } else if (num == HORSE) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_ma.png";
                PieceGroup->AddNewObject(new_piece = new HorsePiece(img, position, color, HORSE * 10));

            } else if (num == CHARIOT) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_ju.png";
                PieceGroup->AddNewObject(new_piece = new ChariotPiece(img, position, color, CHARIOT * 10));

            } else if (num == CANNON) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_pao.png";
                PieceGroup->AddNewObject(new_piece = new CannonPiece(img, position, color, CANNON * 10));

            } else if (num == PAWN) {
                (color == RED) ? img += "red_piece_bing.png" : img += "black_piece_zu.png";
                PieceGroup->AddNewObject(new_piece = new PawnPiece(img, position, color, PAWN * 10));
            }

            if (new_piece) PIECES(row, col) = new_piece; // Store the piece pointer into the Chessboard.
        }
    }
}

void XiangqiScene::Update(float deltaTime) {
    // Winning Condition:
    if ((!RedKing && BlackKing) || (RedKing && !BlackKing)) {
        winner = (RedKing) ? RED : BLACK;
        Engine::GameEngine::GetInstance().ChangeScene("xiangqi_win");
    }

    // RoundWarnings
    if (1 <= warning_tick && warning_tick <= 30 * ALLEGRO_PI) {
        RoundWarning1->Color = al_map_rgba(255, 150, 150, 150 * std::sin(warning_tick / (10 * ALLEGRO_PI)));
        RoundWarning2->Color = al_map_rgba(255, 150, 150, 255 * std::sin(warning_tick / (10 * ALLEGRO_PI)));
        RoundWarning3->Color = al_map_rgba(255, 150, 150, 150 * std::sin(warning_tick / (10 * ALLEGRO_PI)));
        warning_tick++;
    } else {
        RoundWarning1->Color = al_map_rgba(255, 150, 150, 0);
        RoundWarning2->Color = al_map_rgba(255, 150, 150, 0);
        RoundWarning3->Color = al_map_rgba(255, 150, 150, 0);
        warning_tick = 0; // Rest warning_tick
    }
}

void XiangqiScene::Terminate() {
    // AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}

void XiangqiScene::OnMouseDown(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    // Select a piece if there is one:
    if ((button & 1) && !SelectFlag) { 
        const int _col_mouse = x_to_col(mx), _row_mouse = y_to_row(my);
        // Check if (x, y) is valid:
        if (_row_mouse < 0 || ChessboardHeight <= _row_mouse || _col_mouse < 0 || ChessboardWidth <= _col_mouse) return;

        if (STATES(_row_mouse, _col_mouse) * Round > 0) { // If not empty && with correct color:
            SelectedRowCol = std::pair(_row_mouse, _col_mouse); // Jot down the original row and column of the player-selected piece.
            auto selectedPiece = PIECES(_row_mouse, _col_mouse);
            assert(selectedPiece != nullptr);
            preview = selectedPiece->Clone(); // Deep copy of the selected piece!
            UIGroup->AddNewObject(preview);
            SelectFlag = true;

        } else if (STATES(_row_mouse, _col_mouse) * Round < 0) { // Of a different color.
            SelectFlag = false;
            WrongPiece = true;
            warning_tick = 1;
        }

    } else if ((button & 1) && SelectFlag) { // ATTEMPT TO PUT DOWN THE PIECE
        UIGroup->RemoveObject(preview->GetObjectIterator());
        if (preview)
            SelectFlag = false;
    }
    IScene::OnMouseDown(button, mx, my);
}

void XiangqiScene::OnMouseMove(int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::OnMouseMove(mx, my);
    const int _col_mouse = x_to_col(mx), _row_mouse = y_to_row(my);

    if (!preview || !PieceWithinRange(_row_mouse, _col_mouse) || (SelectedRowCol.first == _row_mouse && SelectedRowCol.second == _col_mouse)) {
        return;
    }

    if (preview && PieceWithinRange(SelectedRowCol.first, SelectedRowCol.second) && PIECES(SelectedRowCol.first, SelectedRowCol.second)) {    
        preview->Visible = true;
        preview->Position.x = col_to_x(_col_mouse), preview->Position.y = row_to_y(_row_mouse);
        preview->Tint = al_map_rgba(255, 255, 255, 150);//

        // Change preview color if invalid:
        bool temp = PIECES(SelectedRowCol.first, SelectedRowCol.second)->IsValidMove(SelectedRowCol.first, SelectedRowCol.second, _row_mouse, _col_mouse, ChessboardState);
        if (!temp) {
            preview->Tint = al_map_rgba(255, 100, 100, 150); // Redish
        } else {
            preview->Tint = al_map_rgba(100, 255, 100, 150); // Greenish
        }
    }
}

void XiangqiScene::OnMouseUp(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::OnMouseUp(button, mx, my);
    const int _col_mouse = x_to_col(mx), _row_mouse = y_to_row(my); // The column and row of the target position.

    if (button & 1 && preview && !SelectFlag && !WrongPiece) { // ABOUT TO DONE MOVING A PIECE!
        // Check if the target position (in terms of row-col)
        if (_row_mouse < 0 || ChessboardHeight <= _row_mouse || _col_mouse < 0 || ChessboardWidth <= _col_mouse)
            return;
        
        // blockSize * (col-4) + halfW, blockSize * (row-4.5) + halfH
        std::cout << "[DEBUGGER] _row_selected, _col_selected = " <<  SelectedRowCol.first << "," << SelectedRowCol.second << std::endl;
        int state_selected = STATES(SelectedRowCol.first, SelectedRowCol.second);

        // Check if valid. (Call the method of the chess to see if valid.)
        if (!PIECES(SelectedRowCol.first, SelectedRowCol.second)->IsValidMove(SelectedRowCol.first, SelectedRowCol.second, _row_mouse, _col_mouse, ChessboardState)) {
            return;
        }

        // PIECE MOVEMENT & UPDATE        
        // Case 2 and 1: Eat an enemy || Wander to a no man's land.
        // Case 2: If the target block is occupied by a piece of different color, EAT it!
        std::cout << "[DEBUGGER] Case 2 in OnMouseUp: ";
        std::cout << STATES(_row_mouse, _col_mouse) << " * " << Round << std::endl;
        if (STATES(_row_mouse, _col_mouse) * Round < 0) { // Of different color.
            // Eat the target
            auto ptr = PIECES(_row_mouse, _col_mouse);
            PieceGroup->RemoveObject(ptr->GetObjectIterator()); // Delete the eaten piece from PieceGroup.

            // Case: Eating a king piece.
            if (STATES(_row_mouse, _col_mouse) == RED * KING)       RedKing = false;
            else if (STATES(_row_mouse, _col_mouse) == BLACK *KING) BlackKing = false;
        }

        // Moved to Target
        STATES(_row_mouse, _col_mouse) = state_selected; // Move the selected piece (state) to the target position.
        PIECES(_row_mouse, _col_mouse) = PIECES(SelectedRowCol.first, SelectedRowCol.second);
        PIECES(_row_mouse, _col_mouse)->Position.x = col_to_x(_col_mouse); // As for the actual selectedPiece pointer, change its x.
        PIECES(_row_mouse, _col_mouse)->Position.y = row_to_y(_row_mouse); // As for the actual selectedPiece pointer, change its y.

        // Deleted from Selected Place
        STATES(SelectedRowCol.first, SelectedRowCol.second) = NONE;       // Clear the original selected position's state.
        PIECES(SelectedRowCol.first, SelectedRowCol.second) = nullptr;
        std::cout << "[DEBUGGER] move selectedPiece to the next place." << std::endl;
        
        Round = (Round == RED) ? BLACK : RED; // `Round` flip - for RED to BLACK, and vice versa.
        RoundReminder->Text = ((Round == RED) ? "RED" : "BLACK"); // Change RoundReminder text.
        RoundReminder->Color = ((Round == RED) ? al_map_rgba(255, 0, 0, 255) : al_map_rgba(100, 100, 150, 255));
        RoundWarning2->Text = ((Round == RED) ? "BLACK PIECES" : "RED PIECES"); // Change RoundWarning text.
        std::cout << "[DEBUGGER] round == " << Round << std::endl;
    }
    PrintChessboardState();//
    WrongPiece = false; // Reset `WrongPiece` to default: false.
    OnMouseMove(mx, my);
}