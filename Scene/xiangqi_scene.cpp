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
        for (int col = 0; col < ChessboardWidth; col++) std::cout << std::setw(2) << std::setfill(' ') << ChessboardState[row][col] << " ";
        std::cout << std::endl;
    }
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

    ChessboardGroup->AddNewObject(chessboard = new Engine::Image("xiangqi/xiangqi_chessboard_albert.jpg", halfW, halfH, blockSize * 10, blockSize * 11, 0.5, 0.5));
    // Round Reminder (color)
    UIGroup->AddNewObject(new Engine::Label("Round:", "pirulen.ttf", 40, halfW / 4, halfH / 8, 255, 255, 255, 255, 0.5, 0.5));
    UIGroup->AddNewObject(RoundReminder = new Engine::Label("RED", "pirulen.ttf", 40, halfW / 4, halfH / 8 + 40, 255, 255, 255, 255, 0.5, 0.5));
    RoundReminder->Color = al_map_rgba(255, 0, 0, 255); // Current color: RED
    // Round Warning
    UIGroup->AddNewObject(RoundWarning1 = new Engine::Label("Cannot choose", "pirulen.ttf", 30, w - halfW / 4, halfH / 8, 255, 150, 150, 0, 0.5, 0.5));
    UIGroup->AddNewObject(RoundWarning2 = new Engine::Label("BLACK PIECES", "pirulen.ttf", 36, w - halfW / 4, halfH / 8 + 50, 255, 150, 150, 0, 0.5, 0.5));
    UIGroup->AddNewObject(RoundWarning3 = new Engine::Label("in this round!!", "pirulen.ttf", 30, w - halfW / 4, halfH / 8 + 100, 255, 150, 150, 0, 0.5, 0.5));

    Round = RED;
    preview = nullptr;
    SelectFlag = false;
    SwitchFlag = false;
    RedKing = BlackKing = nullptr;

    // bgmId = AudioHelper::PlayBGM("play.ogg");
}
void XiangqiScene::ReadChessboard() {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    this->ChessboardState.clear();
    
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
    ChessboardState = std::vector<std::vector<int>>(ChessboardHeight, std::vector<int>(ChessboardWidth));
    for (int row = 0; row < ChessboardHeight; row++) {
        PieceColor color = (row < ChessboardHeight / 2) ? BLACK : RED;

        for (int col = 0; col < ChessboardWidth; col++) {
            const int num = mapData[row * ChessboardWidth + col];
            ChessboardState[row][col] = color * num; // Red pieces would generally go up (decrease in y), so I set the state of red pieces to negative.
            
            // Construct all the chess pieces.
            // BLACK at the top, RED at the bottom.
            Engine::Point position(blockSize * (col-4) + halfW, blockSize * (row-4.5) + halfH); // Regular form: i for row, j for column!
            Engine::Point palace_position(col, row);                                            // Palace form
            std::string img = "xiangqi/";
            ChessPiece *new_piece = nullptr;
            if (num == KING) {
                if (color == RED) {
                    img += "red_piece_shuai.png";
                    PieceGroup->AddNewObject(new_piece = new KingPiece(img, position, color, false, KING * 10));
                    RedKing = new_piece;

                } else {
                    img += "black_piece_jiang.png";
                    PieceGroup->AddNewObject(new_piece = new KingPiece(img, position, color, false, KING * 10));
                    BlackKing = new_piece;
                }

            } else if (num == GUARD) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_shi.png";
                PieceGroup->AddNewObject(new_piece = new GuardPiece(img, position, color, false, GUARD * 10));

            } else if (num == ELFNT) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_xiang.png";
                PieceGroup->AddNewObject(new_piece = new ElephantPiece(img, position, color, false, ELFNT * 10));

            } else if (num == HORSE) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_ma.png";
                PieceGroup->AddNewObject(new_piece = new HorsePiece(img, position, color, false, HORSE * 10));

            } else if (num == CHARIOT) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_ju.png";
                PieceGroup->AddNewObject(new_piece = new ChariotPiece(img, position, color, false, CHARIOT * 10));

            } else if (num == CANNON) {
                (color == RED) ? img += "red_" : img += "black_";
                img += "piece_pao.png";
                PieceGroup->AddNewObject(new_piece = new CannonPiece(img, position, color, false, CANNON * 10));

            } else if (num == PAWN) {
                (color == RED) ? img += "red_piece_bing.png" : img += "black_piece_zu.png";
                PieceGroup->AddNewObject(new_piece = new PawnPiece(img, position, color, false, PAWN * 10));
            }

            if (new_piece) PieceMap.insert({palace_position, new_piece}); // Insert the pair (PalacePos, ChessPiece*) into PieceMap.
                                                                          // PalacePos := (_col, _row)
        }
    }

    PrintChessboardState();//
}

void XiangqiScene::Update(float deltaTime) {
    // Winning Condition:
    // if (!RedKing || !BlackKing) {
    //     Engine::GameEngine::GetInstance().ChangeScene("xiangqi_win");
    // }

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

    if ((button & 1) && !SelectFlag) { // START TO SELECT A PIECE (IF THERE IS ONE)
        const int _col = x_to_col(mx), _row = y_to_row(my);
        Engine::Point _targetPos(_col, _row); // Target position col & row

        std::cout << "[DEBUGGER] OnMouseDown: " << _row << "," << _col << std::endl;//
        
        // Check if (x, y) is valid:
        if (_col < 0 || ChessboardWidth <= _col || _row < 0 || ChessboardHeight <= _row)
            return;

        std::cout << "[DEBUGGER] OnMouseDown: ChessboardState[" << _row << "][" << _col << "] == " << ChessboardState[_row][_col] << std::endl;//

        if (ChessboardState[_row][_col] * Round > 0) { // If not empty && same color:
            selectedPiece = (PieceMap.find(_targetPos))->second; // Assign `selectPiece` with an actual chess piece.
            assert(selectedPiece != nullptr);
            std::cout << "[DEBUG] selectedPiece is type: " << typeid(*selectedPiece).name() << std::endl;
            preview = selectedPiece->Clone(); // Deep copy of the selected piece!
            preview->isPreview = true; // Set this as an `isPreview` ChessPiece.
            
            UIGroup->AddNewObject(preview);
            std::cout << "[DEBUGGER] Preview created!" << std::endl;

            SelectFlag = true;

        } else if (ChessboardState[_row][_col] * Round < 0) { // Of a different color.
            SelectFlag = false;
            WrongPiece = true;
            warning_tick = 1;
            std::cout << "[DEBUGGER] OnMouseDown: START TO SELECT A PIECE (IF THERE IS ONE) --> OF A DIFFERENT COLOR" << std::endl;
        }

    } else if ((button & 1) && SelectFlag) { // ATTEMPT TO PUT DOWN THE PIECE
        if (preview) {
            std::cout << "[DEBUGGER] Remove preview from UIGroup!" << std::endl;
            // std::cout << "[DEBUGGER] preview != nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;//
            UIGroup->RemoveObject(preview->GetObjectIterator());

            SelectFlag = false;
        }
    }
    IScene::OnMouseDown(button, mx, my);
}

void XiangqiScene::OnMouseMove(int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::OnMouseMove(mx, my);
    const int _col = x_to_col(mx), _row = y_to_row(my);

    if (!preview || _col < 0 || _col >= ChessboardWidth || _row < 0 || _row >= ChessboardHeight) {
        // std::cout << "[DEBUGGER] OnMouseMove (i): preview != nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;//
        return;
    }
    preview->Visible = true;
    preview->Tint = al_map_rgba(255, 255, 255, 100);
    preview->Position.x = blockSize * (_col-4)   + halfW;
    preview->Position.y = blockSize * (_row-4.5) + halfH;
    // std::cout << "[DEBUGGER] OnMousemove(ii): preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;//
}

void XiangqiScene::OnMouseUp(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::OnMouseUp(button, mx, my);
    const int _col_target = x_to_col(mx), _row_target = y_to_row(my); // Rhe column and row of the target position.
    Engine::Point _target(_col_target, _row_target);

    if (button & 1 && preview && !SelectFlag && !WrongPiece) { // ABOUT TO DONE MOVING A PIECE!
        // blockSize * (col-4) + halfW, blockSize * (row-4.5) + halfH
        const int _col_selected = x_to_col(selectedPiece->Position.x), _row_selected = y_to_row(selectedPiece->Position.y);
        Engine::Point _selected(_col_selected, _row_selected); // PalacePos format: (_col, _row)
        std::cout << "[DEBUGGER] _row_selected, _col_selected = " <<  _row_selected << "," << _col_selected << std::endl;
        int state_selected = ChessboardState[_row_selected][_col_selected];

        // Check if valid. (Call the method of the chess to see if valid.)

        // preview = nullptr;

        // Moving Pieces!
        // Case 1: If the selected piece wanders to a no man's land, we merely move the selected piece to the wanted place.
        // Case 2: If the target block is occupied by a piece of different color, EAT it!
        // Case 3: If the target block is occupied by a piece of the same color, CHANGE `selectedPiece`.
        
        // Case 3
        if (ChessboardState[_row_target][_col_target] * Round > 0) { // Of the same color.
            std::cout << "[DEBUGGER] OnMouseUp: Case 3 - Of the same color!" << std::endl;
            selectedPiece = PieceMap.find(_target)->second;
            std::cout << "[DEBUGGER] OnMouseUp: Case 3-2" << std::endl;
            std::cout << "[DEBUGGER] preview != nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;//
            preview = nullptr;
            std::cout << "[DEBUGGER] OnMouseUp: Case 3-3" << std::endl;
            assert(selectedPiece != nullptr);
            std::cout << "[DEBUG] selectedPiece is type: " << typeid(*selectedPiece).name() << std::endl;
            preview = selectedPiece->Clone();
            std::cout << "[DEBUGGER] OnMouseUp: Case 3-4" << std::endl;
            SelectFlag = true;
            std::cout << "[DEBUGGER] OnMouseUp: Case 3-5" << std::endl;
        }

        // Case 2 and 1
        else {
            // Case 2
            if (ChessboardState[_row_target][_col_target] * Round < 0) { // Of different color.
                // Eat the target
                auto it = PieceMap.find(_target);
                auto pt = it->first;//
                PieceGroup->RemoveObject(it->second->GetObjectIterator()); // Delete the eaten piece from PieceGroup.
                PieceMap.erase(it); // Delete the eaten piece from PieceMap.
                std::cout << "[DEBUGGER] The target chess piece at (" << pt.x << "," << pt.y << ") has been successfully eaten!" << std::endl;
            }

            
            ChessboardState[_row_target][_col_target] = state_selected; // Move the selected piece (state) to the target position.
            ChessboardState[_row_selected][_col_selected] = NONE;       // Clear the original selected position's state.
            std::cout << "[DEBUGGER] ChessboardState[" << _row_selected << "][" << _col_selected << "] == " << ChessboardState[_row_selected][_col_selected] << std::endl;
            selectedPiece->Position.x = col_to_x(_col_target); // As for the actual selectedPiece pointer, change its x.
            selectedPiece->Position.y = row_to_y(_row_target); // As for the actual selectedPiece pointer, change its y.
            PieceMap.erase(_selected);                 // Erase the old data with old selected position as the key.
            PieceMap.insert({_target, selectedPiece}); // Add new data with target position as the key.
            std::cout << "[DEBUGGER] move selectedPiece to the next place." << std::endl;
            
            selectedPiece = nullptr;              // "Release" `selectedPiece`
            Round = (Round == RED) ? BLACK : RED; // `Round` flip - for RED to BLACK, and vice versa.
            RoundReminder->Text = ((Round == RED) ? "RED" : "BLACK"); // Change RoundReminder text.
            RoundReminder->Color = ((Round == RED) ? al_map_rgba(255, 0, 0, 255) : al_map_rgba(100, 100, 150, 255));
            RoundWarning2->Text = ((Round == RED) ? "BLACK PIECES" : "RED PIECES"); // Change RoundWarning text.
        }
        // std::cout << "[DEBUGGER] OnMouseUp: preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;//
        std::cout << "[DEBUGGER] round == " << Round << std::endl;

        OnMouseMove(mx, my);
    }
    PrintChessboardState();
    WrongPiece = false; // Reset `WrongPiece` to default: false.
}