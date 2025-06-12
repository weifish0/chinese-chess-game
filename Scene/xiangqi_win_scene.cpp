#include "xiangqi_win_scene.hpp"

void XiangqiWinScene::Initialize() {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    // Winning Label (Text)
    std::string win_str = (winner == HONG) ? "HONG" : "HEI";
    win_str += " wins!! :D";
    Engine::Label *lbl;
    AddNewObject(lbl = new Engine::Label(win_str, "pirulen.ttf", 40, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5));
    lbl->Color = (winner == HONG) ? al_map_rgba(255, 0, 0, 255) : al_map_rgba(100, 100, 150, 255);

    // Back-to-Playground Button
}

void XiangqiWinScene::Update(float deltatime) {
}

void XiangqiWinScene::Terminate() {
}