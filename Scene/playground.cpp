#include "playground.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>


void Playground::Initialize() {
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/playground.png");
    
    // 載入暗棋建築圖片
    anqi_house = Engine::Resources::GetInstance().GetBitmap("playground/anqi_house.png");
    
    // 載入象棋建築圖片
    xiangqi_house = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_house.png");

    // 創建玩家
    player = new Player();
    AddNewObject(player);

    // 初始化鏡頭位置到玩家位置
    camera_x = player->getX() - VIEWPORT_WIDTH / 2;
    camera_y = player->getY() - VIEWPORT_HEIGHT / 2;
}

void Playground::Terminate() {
    IScene::Terminate();
}

void Playground::Draw() const {
    float scale_x = SCREEN_RIGHT / VIEWPORT_WIDTH;
    float scale_y = SCREEN_BOTTOM / VIEWPORT_HEIGHT;

    // 繪製背景（只繪製視窗範圍內的部分，並縮放到全螢幕）
    if (background) {
        al_draw_scaled_bitmap(
            background.get(),
            camera_x, camera_y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, // 來源區域
            0, 0, SCREEN_RIGHT, SCREEN_BOTTOM,                   // 目標區域（全螢幕）
            0
        );
    }
    
    // 繪製暗棋建築
    if (anqi_house) {
        float draw_x = (ANQI_HOUSE_X - camera_x) * scale_x;
        float draw_y = (ANQI_HOUSE_Y - camera_y) * scale_y;
        al_draw_scaled_bitmap(
            anqi_house.get(),
            0, 0, al_get_bitmap_width(anqi_house.get()), al_get_bitmap_height(anqi_house.get()),
            draw_x, draw_y, ANQI_HOUSE_SIZE, ANQI_HOUSE_SIZE, 
            0
        );
    }

    if (xiangqi_house) {
        float draw_x = (XIANGQI_HOUSE_X - camera_x) * scale_x;
        float draw_y = (XIANGQI_HOUSE_Y - camera_y) * scale_y;
        al_draw_scaled_bitmap(
            xiangqi_house.get(),
            0, 0, al_get_bitmap_width(xiangqi_house.get()), al_get_bitmap_height(xiangqi_house.get()),
            draw_x, draw_y, XIANGQI_HOUSE_SIZE, XIANGQI_HOUSE_SIZE, 
            0
        );
    }
    
    // 繪製玩家（固定在螢幕正中央，並縮放）
    if (player) {
        float sprite_width = player->getSize();
        float sprite_height = player->getSize();
        float draw_x = (player->getX() - camera_x - sprite_width / 2) * scale_x;
        float draw_y = (player->getY() - camera_y - sprite_height / 2) * scale_y;
        player->Draw(draw_x, draw_y, scale_x, scale_y);
    }
}

void Playground::UpdateCamera() {
    if (!player) return;
    // std::cout << "camera(" << camera_x << ", " << camera_y << ")" << std::endl;

    // 目標鏡頭位置
    camera_x  = player->getX() - VIEWPORT_WIDTH / 2;
    camera_y = player->getY() - VIEWPORT_HEIGHT / 2;

    // 限制鏡頭不超出地圖邊界
    camera_x = std::max(0.0f, std::min(camera_x, SCREEN_RIGHT - VIEWPORT_WIDTH));
    camera_y = std::max(0.0f, std::min(camera_y, SCREEN_BOTTOM - VIEWPORT_HEIGHT));
}

void Playground::Update(float deltaTime) {
    player->Update(deltaTime);

    // 更新鏡頭位置
    UpdateCamera();
    
    // 獲取玩家當前位置
    float playerX = player->getX();
    float playerY = player->getY();
    float playerSize = player->getSize();
    
    // 檢查玩家是否靠近暗棋建築
    float distance = std::sqrt(std::pow(playerX - ANQI_HOUSE_X, 2) + std::pow(playerY - ANQI_HOUSE_Y, 2));
    if (distance < 100 && Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_SPACE)) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }

    // 檢查玩家是否靠近象棋建築
    float distance2 = std::sqrt(std::pow(playerX - XIANGQI_HOUSE_X, 2) + std::pow(playerY - XIANGQI_HOUSE_Y, 2));
    if (distance2 < 100 && Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_SPACE)) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
    
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

// 新增：處理滑鼠事件
void Playground::OnMouseDown(int button, int mx, int my) {
    std::cout << "Mouse Click: screen(" << mx << ", " << my << ")" << std::endl;
}
