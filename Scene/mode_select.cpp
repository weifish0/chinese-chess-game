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

#include "Scene/anqi_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "mode_select.hpp"



void ModeSelectScene::Initialize(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", halfW - 400, halfH / 2 -200, 800, 200);
    btn->SetOnClickCallback(std::bind(&ModeSelectScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("單人模式", "font2.ttc", 120, halfW, halfH / 2 - 100, 0, 0, 102, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", halfW - 400, halfH / 2 + 250, 800, 200);
    btn->SetOnClickCallback(std::bind(&ModeSelectScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("雙人模式", "font2.ttc", 120, halfW, halfH / 2 + 350, 0, 0, 102, 255, 0.5, 0.5));

    // btn = new Engine::ImageButton("start/block.png", "start/dirt.png", halfW - 400, halfH / 2 + 300, 800, 200);
    // btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    // AddNewControlObject(btn);
    // AddNewObject(new Engine::Label("開始", "font2.ttc", 140, halfW, halfH / 2 + 400, 0, 0, 102, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", halfW - 400, halfH / 2 + 700, 800, 200);
    btn->SetOnClickCallback(std::bind(&ModeSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("返回", "font2.ttc", 120, halfW, halfH / 2 + 800, 0, 0, 102, 255, 0.5, 0.5));

}
void ModeSelectScene::Terminate(){
    IScene::Terminate();
}
void ModeSelectScene::PlayOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void ModeSelectScene::BackOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("anqi_start");
}