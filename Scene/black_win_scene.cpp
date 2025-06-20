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

#include "Scene/black_win_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"


void Black_Win_Scene::Initialize(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;

    AddNewObject(new Engine::Label("<< 黑方勝利 >>", "font2.ttc", 150, halfW, halfH / 3 -20 , 116, 96, 182, 255, 0.5, 0.5));
    AddNewObject(new Engine::Image("black_win.png", 1333-432, 450 , 864 , 864 , 0, 0));

    btn = new Engine::ImageButton("start/block.png", "start/dirt.png", w - 500, halfH *3 / 2 + 100, 400, 200);
    btn->SetOnClickCallback(std::bind(&Black_Win_Scene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("返回", "font2.ttc", 140, w-300, halfH *3 / 2 + 200, 14 , 37 , 67 , 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayBGM("black_win_music.ogg");

}

void Black_Win_Scene::Terminate(){
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
    
}
void Black_Win_Scene::BackOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("anqi_start");
}