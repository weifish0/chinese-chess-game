#include "playground.hpp"
#include "Engine/GameEngine.hpp"

void Playground::Initialize() {
    // 創建玩家
    player = new Player();
    AddNewObject(player);
}

void Playground::Terminate() {
    IScene::Terminate();
}

void Playground::Update(float deltaTime) {
    player->Update(deltaTime);
    // 處理輸入
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_W)) {
        player->moveUp();
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S)) {
        player->moveDown();
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A)) {
        player->moveLeft();
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D)) {
        player->moveRight();
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_ESCAPE)) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
}