#include <algorithm>
#include <allegro5/allegro.h>
#include <iostream>
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
    for (int i = 0; i < ChessboardHeight; i++) {
        PieceColor color = (i < ChessboardHeight / 2) ? BLACK : RED;

        for (int j = 0; j < ChessboardWidth; j++) {
            const int num = mapData[i * ChessboardWidth + j];
            ChessboardState[i][j] = color * num; // Red pieces would generally go up (decrease in y), so I set the state of red pieces to negative.
            
            // Construct all the chess pieces.
            // BLACK at the top, RED at the bottom.
            Engine::Point position(blockSize * (j-4) + halfW, blockSize * (i-4.5) + halfH); // i for row, j for column!
            Engine::Point palace_position(j, i);
            std::string img = "xiangqi_scene/";
            ChessPiece *new_piece = nullptr;
            if (num == KING) {
                (color == RED) ? img += "red_piece_shuai.png" : img += "black_piece_jiang.png";
                PieceGroup->AddNewObject(new_piece = new KingPiece(img, position, color, false, KING * 10));

            } 
            else if (num == GUARD) {
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

            if (new_piece) PieceMap.insert({palace_position, new_piece});
        }
    }
}

void XiangqiScene::OnMouseDown(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    if ((button & 1) && !SelectFlag) { // ABOUT TO SELECT A PIECE
        const int x = std::floor((mx + 0.5 * blockSize - halfW) / blockSize) + 4;
        const int y = std::floor((my                   - halfH) / blockSize) + 5;
        
        // Check if (x, y) is valid:
        if (!(0 <= x && x < ChessboardWidth && 0 <= y && y < ChessboardWidth))
            return;

        std::cout << "[DEBUGGER] x, y = " << x << "," << y << std::endl;
        Engine::Point targetPalacePos(x, y);
        if (ChessboardState[x][y] != NONE) {
            preview = new ChessPiece(*(PieceMap.find(targetPalacePos)->second)); // Deep copy of the selected piece!
            preview->isPreview = true; // Set this as an `isPreview` ChessPiece.
            
            UIGroup->AddNewObject(preview);
            std::cout << "[DEBUGGER] Preview created!" << std::endl;
        }
        SelectFlag = true;

    } else if ((button & 1) && SelectFlag) { // ABOUT TO PUT DOWN THE PIECE
        std::cout << "[DEBUGGER] Remove preview from UIGroup!" << std::endl;
        std::cout << "[DEBUGGER] preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
        UIGroup->RemoveObject(preview->GetObjectIterator());
        std::cout << "[DEBUGGER] Preview removed!" << std::endl;
        preview = nullptr;
        std::cout << "[DEBUGGER] Preview deleted!" << std::endl;
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
    const int x = std::floor((mx + 0.5 * blockSize - halfW) / blockSize) + 4;
    const int y = std::floor((my                   - halfH) / blockSize) + 5;

    if (!preview || x < 0 || x >= ChessboardWidth || y < 0 || y >= ChessboardHeight) {
        std::cout << "[DEBUGGER] OnMouseMove (i): preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
        return;
    }
    preview->Visible = true;
    preview->Tint = al_map_rgba(255, 255, 255, 100);
    preview->Position.x = blockSize * (x-4)   + halfW;
    preview->Position.y = blockSize * (y-4.5) + halfH;
    std::cout << "[DEBUGGER] OnMousemove(ii): preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
}

void XiangqiScene::OnMouseUp(int button, int mx, int my) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::OnMouseUp(button, mx, my);
    const int x = std::floor((mx + 0.5 * blockSize - halfW) / blockSize) + 4;
    const int y = std::floor((my                   - halfH) / blockSize) + 5;
    if (button & 1) {
        int selectedPieceState = ChessboardState[selectedPiecePos.x / blockSize][selectedPiecePos.y / blockSize];
        if (!preview) return;
        // Check if valid.
        
        // Remove Preview. (Call the method of the chess to see if valid.)
        
        
        // Construct a real chess piece.

        // Keep responding when paused.
        preview->Update(0);

        // Remove preview.
        // preview = nullptr;

        ChessboardState[x][y] = selectedPieceState;
        ChessboardState[selectedPiecePos.x / blockSize][selectedPiecePos.y / blockSize] = NONE;
        OnMouseMove(mx, my);

        std::cout << "[DEBUGGER] OnMouseUp: preview == nullptr ? " << ((preview) ? "YES" : "NAH") << std::endl;
    }
}