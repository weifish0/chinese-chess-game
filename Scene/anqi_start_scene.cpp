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

#include "Scene/anqi_start_scene.hpp"
#include "Scene/anqi_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void Anqi_StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    AddNewObject(new Engine::Image("start.png", 0, 0 , 2666 , 1500 , 0, 0));
    btn = new Engine::ImageButton("block6.png", "block6.png", halfW - 450, 200, 900, 200);
    btn->SetOnClickCallback(std::bind(&Anqi_StartScene::PlayOnClick, this, 0));
    AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("開始", "font2.ttc", 140, halfW, 880, 255,255,255 , 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("對戰暗棋", "font2.ttc", 200, halfW, halfH / 3 + 50, 255, 255, 255, 255, 0.5, 0.5));
    // auto font = al_load_font("pirulen.ttf", 120, 0);
    // if (!font) std::cerr << "Font load failed!" << std::endl;

    btn = new Engine::ImageButton("block2.png", "block5.png", halfW - 200, 800, 400, 200);
    btn->SetOnClickCallback(std::bind(&Anqi_StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("開始", "font2.ttc", 140, halfW, 900, 255,255,255 , 255, 0.5, 0.5));

    btn = new Engine::ImageButton("block2.png", "block5.png", halfW - 200, 1200, 400, 200);
    btn->SetOnClickCallback(std::bind(&Anqi_StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("設置", "font2.ttc", 140, halfW, 1300, 255,255,255, 255, 0.5, 0.5));
    ALLEGRO_MOUSE_STATE mouse;
    al_get_mouse_state(&mouse);
    bool mouseNowDown = al_mouse_button_down(&mouse, 1);
    mouseJustClicked = (mouseNowDown && !lastMouseDown);  // Rising edge
    lastMouseDown = mouseNowDown;

    btn = new Engine::ImageButton("block2.png", "block5.png", 2366, 1350, 300, 150);
    btn->SetOnClickCallback(std::bind(&Anqi_StartScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("返回", "font2.ttc", 100, 2666-150, 1425, 255,255,255 , 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayBGM("anqi_start_music.ogg");
}
void Anqi_StartScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}
void Anqi_StartScene::PlayOnClick(int stage) {
    if(stage)
        Engine::GameEngine::GetInstance().ChangeScene("play");
}
void Anqi_StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("setting");
}
void Anqi_StartScene::BackOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("playground");
}