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

#include "Scene/anqi_setting_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "anqi_scene.hpp"
#include <cmath>
int decide = 0;
int x1,x2;
int pointer_tick = -45;
Engine::Image *pointer1;
std::string close_img = "unknown.png";
void AnqiSettingScene::Update(float deltaTime){
    close_img = decide?"unknown3.png":"unknown.png";
    int x = decide?x2:x1;
    pointer1->Position.x = x + 10*sin(2*pi*pointer_tick/45);
    pointer_tick++;
    if(pointer_tick>45){
        pointer_tick = -45;
    }
}
void AnqiSettingScene::Initialize() {
    pointer_tick = -45;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    // btn = new Engine::ImageButton("block6.png", "block6.png", halfW - 450, 200, 900, 200);
    // btn->SetOnClickCallback(std::bind(&AnqiSettingScene::PlayOnClick, this, 0));
    // AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("開始", "font2.ttc", 140, halfW, 880, 255,255,255 , 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("請選擇棋子樣式", "font2.ttc", 150, halfW, halfH / 4, 255, 255, 255, 255, 0.5, 0.5));

    AddNewObject(new Engine::Image("black_king.png", 400, 450 , 300 , 300 , 0, 0));
    AddNewObject(new Engine::Image("red_king.png",800, 450 , 300 , 300 , 0, 0));
    AddNewObject(new Engine::Image("unknown.png", 600, 800 , 300 , 300 , 0, 0));
    
    AddNewObject(new Engine::Image("black_king3.png", 1400, 450 , 300 , 300 , 0, 0));
    AddNewObject(new Engine::Image("red_king3.png", 1800, 450 , 300 , 300 , 0, 0));
    AddNewObject(new Engine::Image("unknown3.png", 1600, 800 , 300 , 300 , 0, 0));
    x1 = 390;
    x2 = 1390;
    pointer1 = new Engine::Image("pointer2.png",x1 , 1225 , 200 , 200 , 0, 0);
    AddNewObject(pointer1);
    // auto font = al_load_font("pirulen.ttf", 120, 0);
    // if (!font) std::cerr << "Font load failed!" << std::endl;

    btn = new Engine::ImageButton("block2.png", "block5.png", 600, 1250, 300,150);
    btn->SetOnClickCallback(std::bind(&AnqiSettingScene::SettingsOnClick, this, 0));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("選擇", "font2.ttc", 100, 750, 1325, 255,255,255 , 255, 0.5, 0.5));

    btn = new Engine::ImageButton("block2.png", "block5.png", 1600, 1250, 300,150);
    btn->SetOnClickCallback(std::bind(&AnqiSettingScene::SettingsOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("選擇", "font2.ttc", 100, 1750, 1325, 255,255,255, 255, 0.5, 0.5));
    // ALLEGRO_MOUSE_STATE mouse;
    // al_get_mouse_state(&mouse);
    // bool mouseNowDown = al_mouse_button_down(&mouse, 1);
    // mouseJustClicked = (mouseNowDown && !lastMouseDown);  // Rising edge
    // lastMouseDown = mouseNowDown;

    btn = new Engine::ImageButton("block2.png", "block5.png", 2266, 1250, 300, 150);
    btn->SetOnClickCallback(std::bind(&AnqiSettingScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("返回", "font2.ttc", 100, 2666-250, 1325, 255,255,255 , 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayBGM("setting_music.ogg");
}
void AnqiSettingScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}
void AnqiSettingScene::SettingsOnClick(int id) {
    decide = id;
}
void AnqiSettingScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("anqi_start");
}