#include "xiangqi_win_scene.hpp"

void XiangqiWinScene::Initialize() {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    // Winning Label (Text)
    Engine::Label *lbl;
    if (winner == HONG) {
        lbl = new Engine::Label("RED WINS!! :D", "pirulen.ttf", 40, halfW, halfH / 2, 255, 0, 0, 255, 0.5, 0.5);

    } else if (winner == HEI) {
        lbl = new Engine::Label("BLACK WINS!! :D", "pirulen.ttf", 40, halfW, halfH / 2, 100, 100, 150, 255, 0.5, 0.5);

    } else if (winner == NIL) {
        lbl = new Engine::Label("PEACE ^_^", "pirulen.ttf", 40, halfW, halfH / 2, 255, 0, 0, 255, 0.5, 0.5);
    }
    AddNewObject(lbl);

    // Play-Again Button
    AgainBtn = new Engine::ImageButton("xiangqi/floor.png", "xiangqi/dirt.png", halfW, halfH, 400, 100);
    AgainBtn->SetOnClickCallback(std::bind(&XiangqiWinScene::AgainOnClick, this));
    AddNewControlObject(AgainBtn);
    AddNewObject(new Engine::Label("PLAY AGAIN", "pirulen.ttf", 48, halfW, halfH, 255, 255, 255, 255, 0.5, 0.5));
    // Exit (Back-to-Playground) Button
    PlaygroundBtn = new Engine::ImageButton("xiangqi/floor.png", "xiangqi/dirt.png", halfW, halfH * 1.5, 400, 100);
    PlaygroundBtn->SetOnClickCallback(std::bind(&XiangqiWinScene::PlaygroundOnClick, this));
    AddNewControlObject(PlaygroundBtn);
    AddNewObject(new Engine::Label("EXIT", "pirulen.ttf", 48, halfW, halfH * 1.5, 255, 255, 255, 255, 0.5, 0.5));
}

void XiangqiWinScene::Update(float deltatime) {
}

void XiangqiWinScene::Terminate() {
    IScene::Terminate();
}

void XiangqiWinScene::AgainOnClick() {
    Terminate();
    Engine::GameEngine::GetInstance().ChangeScene("xiangqi");
}

void XiangqiWinScene::PlaygroundOnClick() {
    Terminate();
    Engine::GameEngine::GetInstance().ChangeScene("playground");
}