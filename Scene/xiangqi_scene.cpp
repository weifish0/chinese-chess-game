#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "Scene/xiangqi_scene.hpp"

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

    AddNewObject(PieceGroup = new Group());

    AddNewObject(new Engine::Image("xiangqi_scene/xiangqi_chessboard_albert.jpg", halfW, halfH, blockSize * 10, blockSize * 11, 0.5, 0.5));
    // PieceGroup->AddNewObject(new Engine::Image("red_piece_shuai.png", halfW, halfH, 50, 50, 0.5, 0.5));
    AddNewObject(new Engine::Image("xiangqi_scene/red_piece_shuai.png", halfW, h - blockSize, blockSize * 0.9, blockSize * 0.9, 0.5, 0.5));
    
}