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

#include "Scene/red_win_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"


void Red_Win_Scene::Initialize(){
    
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    AddNewObject(new Engine::Label("<< 紅方勝利 >>", "font2.ttc", 150, halfW, halfH / 3 -20, 223, 21, 21, 255, 0.5, 0.5));
    AddNewObject(new Engine::Image("red_win.png", 1333-450 , 450 , 900 , 864 , 0, 0));

    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", w - 500, halfH *3 / 2 + 100, 400, 200);
    btn->SetOnClickCallback(std::bind(&Red_Win_Scene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("返回", "font2.ttc", 140, w-300, halfH *3 / 2 + 200, 14 , 37 , 67 , 255, 0.5, 0.5));
    
}

void Red_Win_Scene::Terminate(){
    IScene::Terminate();

}
void Red_Win_Scene::BackOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("anqi_start");
}