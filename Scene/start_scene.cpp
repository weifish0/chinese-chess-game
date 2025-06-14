#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"

#include "Scene/start_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    
    // 載入背景圖片
    AddNewObject(new Engine::Image("start-bg.png", 0, 0, 2666, 1500, 0, 0));
    AddNewObject(new Engine::Label("紅", "font2.ttc", 300, halfW - 300, halfH / 3 + 50, 0, 51, 102, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("與", "font2.ttc", 300, halfW, halfH / 3 + 50, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("黑", "font2.ttc", 300, halfW + 300, halfH / 3 + 50, 255, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("block2.png", "block5.png", halfW - 225, 1200, 450, 200);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("開始冒險", "font2.ttc", 100, halfW, 1300, 255, 255, 255, 255, 0.5, 0.5));
}


void StartScene::PlayOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("playground");//playground
}

void StartScene::Terminate() {
    IScene::Terminate();
}