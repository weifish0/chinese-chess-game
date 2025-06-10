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

int x_to_col(float x);
int y_to_row(float y);

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

    Engine::Image *chessboard;
    ChessboardGroup->AddNewObject(chessboard = new Engine::Image("xiangqi_scene/xiangqi_chessboard_albert.jpg", halfW, halfH, blockSize * 10, blockSize * 11, 0.5, 0.5));
    round = RED;
    preview = nullptr;
    SelectFlag = false;
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
            std::string img = "xiangqi_scene/";
            ChessPiece *new_piece = nullptr;
            if (num == KING) {
                (color == RED) ? img += "red_piece_shuai.png" : img += "black_piece_jiang.png";
                PieceGroup->AddNewObject(new_piece = new KingPiece(img, position, color, false, KING * 10));

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
        }
    }

    // Debugger
    for (int row = 0; row < ChessboardHeight; row++) {
        for (int col = 0; col < ChessboardWidth; col++) std::cout << std::setw(2) << std::setfill(' ') << ChessboardState[row][col] << " ";
        std::cout << std::endl;
    }
}

void XiangqiScene::OnMouseDown(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    if ((button & 1) && !SelectFlag) { // SELECT A PIECE (IF THERE IS ONE)
        const int _col = std::floor((mx + 0.5 * blockSize - halfW) / blockSize) + 4;
        const int _row = std::floor((my                   - halfH) / blockSize) + 5;
        Engine::Point _targetPos(_col, _row);
        
        // Check if (x, y) is valid:
        if (0 > _col || _col >= ChessboardWidth || 0 > _row || _row >= ChessboardWidth)
            return;

        std::cout << "[DEBUGGER] x,y = " << _row << "," << _col << std::endl;
        std::cout << "[DEBUGGER] ChessboardState[" << _row << "][" << _col << "] == " << ChessboardState[_row][_col] << std::endl;

        if (ChessboardState[_row][_col] != NONE && ChessboardState[_row][_col] * round > 0) { // If not empty && same color:
            selectedPiece = (PieceMap.find(_targetPos))->second; // Assign `selectPiece` with an actual chess piece.
            preview = new ChessPiece(*selectedPiece); // Deep copy of the selected piece!
            preview->isPreview = true; // Set this as an `isPreview` ChessPiece.
            
            UIGroup->AddNewObject(preview);
            std::cout << "[DEBUGGER] Preview created!" << std::endl;

            SelectFlag = true;
        }

    } else if ((button & 1) && SelectFlag) { // ABOUT TO PUT DOWN THE PIECE
        if (preview) {
            std::cout << "[DEBUGGER] Remove preview from UIGroup!" << std::endl;
            std::cout << "[DEBUGGER] preview != nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
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
    const int _col = std::floor((mx + 0.5 * blockSize - halfW) / blockSize) + 4;
    const int _row = std::floor((my                   - halfH) / blockSize) + 5;

    if (!preview || _col < 0 || _col >= ChessboardWidth || _row < 0 || _row >= ChessboardHeight) {
        std::cout << "[DEBUGGER] OnMouseMove (i): preview != nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
        return;
    }
    preview->Visible = true;
    preview->Tint = al_map_rgba(255, 255, 255, 100);
    preview->Position.x = blockSize * (_col-4)   + halfW;
    preview->Position.y = blockSize * (_row-4.5) + halfH;
    std::cout << "[DEBUGGER] OnMousemove(ii): preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
}

void XiangqiScene::OnMouseUp(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::OnMouseUp(button, mx, my);
    const int _col = std::floor((mx + 0.5 * blockSize - halfW) / blockSize) + 4; // _x for Palace system
    const int _row = std::floor((my                   - halfH) / blockSize) + 5; // _y for Palace system

    if (button & 1 && !SelectFlag) { // ABOUT TO DONE MOVING A PIECE!
        // blockSize * (col-4) + halfW, blockSize * (row-4.5) + halfH
        const int _col_selected =         (selectedPiece->Position.x - halfW) /         blockSize + 4;
        const int _row_selected = (float) (selectedPiece->Position.y - halfH) / (float) blockSize + 4.5;
        std::cout << "[DEBUGGER] _row_selected, _col_selected = " <<  _row_selected << "," << _col_selected << std::endl;
        int state_selected = ChessboardState[_row_selected][_col_selected];

        // Check if valid. (Call the method of the chess to see if valid.)        
        
        // Construct a real chess piece.

        // Keep responding when paused.
        preview->Update(0);

        // Remove preview.
        preview = nullptr;
        // preview = nullptr;

        ChessboardState[_row][_col] = state_selected;
        ChessboardState[_row_selected][_col_selected] = NONE;
        selectedPiece->Position.x = blockSize * (_col - 4)   + halfW;
        selectedPiece->Position.y = blockSize * (_row - 4.5) + halfH;
        std::cout << "[DEBUGGER] move selectedPiece to the next place." << std::endl;

        // Since after so we are not selecting temporarily, release selectedPiece.
        selectedPiece = nullptr;
        round = (round == RED) ? BLACK : RED;

        std::cout << "[DEBUGGER] OnMouseUp: preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
        std::cout << "[DEBUGGER] round == " << round << std::endl;

        OnMouseMove(mx, my);
    }
}