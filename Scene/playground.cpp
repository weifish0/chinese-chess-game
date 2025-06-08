#include "playground.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>

void Playground::Initialize() {
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/playground.png");
    
    // 創建玩家
    player = new Player();
    AddNewObject(player);
}

void Playground::Terminate() {
    IScene::Terminate();
}

void Playground::Draw() const {
    // 繪製背景
    if (background) {
        std::cout << "background: " << background.get() << std::endl;
        al_draw_bitmap(background.get(), 0, 0, 0);
    }
    
    // 直接繪製玩家，而不是調用 IScene::Draw()
    if (player) {
        player->Draw();
    }
}

void Playground::Update(float deltaTime) {
    player->Update(deltaTime);
    
    // 獲取玩家當前位置
    float playerX = player->getX();
    float playerY = player->getY();
    float playerSize = player->getSize();
    
    // 處理輸入並檢查邊界
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_W)) {
        // 檢查上邊界
        if (playerY - playerSize/2 > SCREEN_TOP) {
            player->moveUp();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S)) {
        // 檢查下邊界
        if (playerY + playerSize/2 < SCREEN_BOTTOM) {
            player->moveDown();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A)) {
        // 檢查左邊界
        if (playerX - playerSize/2 > SCREEN_LEFT) {
            player->moveLeft();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D)) {
        // 檢查右邊界
        if (playerX + playerSize/2 < SCREEN_RIGHT) {
            player->moveRight();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_ESCAPE)) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
}