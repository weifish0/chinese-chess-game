#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <ostream>
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

    AddNewObject(new Engine::Label("對戰暗棋", "font2.ttc", 300, halfW, halfH / 3 + 50, 102, 204, 255, 255, 0.5, 0.5));
    // auto font = al_load_font("pirulen.ttf", 120, 0);
    // if (!font) std::cerr << "Font load failed!" << std::endl;

    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", halfW - 400, halfH / 2 + 300, 800, 200);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("開始", "font2.ttc", 140, halfW, halfH / 2 + 400, 14 , 37 , 67 , 255, 0.5, 0.5));

    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", halfW - 400, halfH * 3 / 2 - 50, 800, 200);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("設置", "font2.ttc", 140, halfW, halfH * 3 / 2 +50, 14, 37, 67, 255, 0.5, 0.5));
}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("mode_select");
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("setting");
}