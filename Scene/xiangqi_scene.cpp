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
#include "ChessPiece/KingPiece.hpp"

#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"

#include "UI/Component/Image.hpp"

class XiangqiScene;
const float blockSize = (float) 850 / (float) 11; // Block size on the chessboard

void XiangqiScene::Initialize() {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;
    
    // Add groups from bottom to top.
    AddNewObject(UIGroup = new Group());
    AddNewObject(PieceGroup = new Group());

    // Read Chessboard txt file
    ReadChessboard();

    Engine::Image *chessboard;
    UIGroup->AddNewObject(chessboard = new Engine::Image("xiangqi_scene/xiangqi_chessboard_albert.jpg", halfW, halfH, blockSize * 10, blockSize * 11, 0.5, 0.5));
    // chessboard->Visible = false;
    // AddNewObject(new Engine::Image("xiangqi_scene/red_piece_shuai.png", halfW, h - blockSize, blockSize * 0.9, blockSize * 0.9, 0.5, 0.5));
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
        std::cout << "[DEBUGGER] c = " << c << std::endl;
        std::cout << ", mapData.size() = " << static_cast<int>(mapData.size()) << std::endl;
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
            ChessboardState[i][j] = num;
            
            // Construct all the chess pieces.
            // BLACK at the top, RED at the bottom.
            Engine::Point position(blockSize * (i-4) + halfW, blockSize * j + halfH);
            std::string img = "xiangqi_scene/";
            if (num == KING) {
                std::cout << "[DEBUGGER] i, j = " << i << "," << j << std::endl;
                (color == RED) ? img += "red_piece_shuai.png" : img += "black_piece_jiang.png";
                PieceGroup->AddNewObject(new KingPiece(img, position, color, false, 100));

            }
        }
    }

    // Debugging
    for (int i=0;i<ChessboardHeight;i++) {
        for(int j=0;j<ChessboardWidth;j++) std::cout << ChessboardState[i][j] << " ";
        std::cout << std::endl;
    }
}