#include "playground.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>


void Playground::Initialize() {
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/playground.png");
    
    // 載入建築圖片
    auto anqi_house_img = Engine::Resources::GetInstance().GetBitmap("playground/anqi_house.png");
    auto xiangqi_house_img = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_house.png");
    
    // 初始化建築物
    buildings.push_back(Building(ANQI_HOUSE_X, ANQI_HOUSE_Y, HOUSE_SIZE, HOUSE_SIZE, anqi_house_img, "anqi_house"));
    buildings.push_back(Building(XIANGQI_HOUSE_X, XIANGQI_HOUSE_Y, HOUSE_SIZE, HOUSE_SIZE, xiangqi_house_img, "xiangqi_house"));
    
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

    // 繪製背景
    if (background) {
        al_draw_scaled_bitmap(
            background.get(),
            camera_x, camera_y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT,
            0, 0, SCREEN_RIGHT, SCREEN_BOTTOM,
            0
        );
    }
    
    // 繪製所有建築物
    for (const auto& building : buildings) {
        float draw_x = (building.x - camera_x - building.width/2) * scale_x;
        float draw_y = (building.y - camera_y - building.height/2) * scale_y;
        al_draw_scaled_bitmap(
            building.image.get(),
            0, 0, al_get_bitmap_width(building.image.get()), al_get_bitmap_height(building.image.get()),
            draw_x, draw_y, building.width * scale_x, building.height * scale_y,
            0
        );
    }
    
    // 繪製玩家
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

bool Playground::CheckBuildingCollision(float newX, float newY) {
    for (const auto& building : buildings) {
        if (building.IsColliding(newX, newY, player->getSize())) {
            return true;
        }
    }
    return false;
}

void Playground::Update(float deltaTime) {
    player->Update(deltaTime);
    UpdateCamera();
    
    float playerX = player->getX();
    float playerY = player->getY();
    float playerSize = player->getSize();
    float moveSpeed = player->getSpeed();
    
    // 處理輸入並檢查碰撞
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_W)) {
        float newY = playerY - moveSpeed;
        // 先檢查垂直移動
        if (newY - playerSize/2 > SCREEN_TOP && !CheckBuildingCollision(playerX, newY)) {
            player->moveUp();
        } else {
            // 如果垂直移動會碰撞，嘗試水平移動
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A) && 
                !CheckBuildingCollision(playerX - moveSpeed, playerY)) {
                player->moveLeft();
            }
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D) && 
                !CheckBuildingCollision(playerX + moveSpeed, playerY)) {
                player->moveRight();
            }
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S)) {
        float newY = playerY + moveSpeed;
        if (newY + playerSize/2 < SCREEN_BOTTOM && !CheckBuildingCollision(playerX, newY)) {
            player->moveDown();
        } else {
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A) && 
                !CheckBuildingCollision(playerX - moveSpeed, playerY)) {
                player->moveLeft();
            }
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D) && 
                !CheckBuildingCollision(playerX + moveSpeed, playerY)) {
                player->moveRight();
            }
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A)) {
        float newX = playerX - moveSpeed;
        if (newX - playerSize/2 > SCREEN_LEFT && !CheckBuildingCollision(newX, playerY)) {
            player->moveLeft();
        } else {
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_W) && 
                !CheckBuildingCollision(playerX, playerY - moveSpeed)) {
                player->moveUp();
            }
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S) && 
                !CheckBuildingCollision(playerX, playerY + moveSpeed)) {
                player->moveDown();
            }
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D)) {
        float newX = playerX + moveSpeed;
        if (newX + playerSize/2 < SCREEN_RIGHT && !CheckBuildingCollision(newX, playerY)) {
            player->moveRight();
        } else {
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_W) && 
                !CheckBuildingCollision(playerX, playerY - moveSpeed)) {
                player->moveUp();
            }
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S) && 
                !CheckBuildingCollision(playerX, playerY + moveSpeed)) {
                player->moveDown();
            }
        }
    }
    
    // 檢查是否靠近建築物並按下空白鍵
    for (const auto& building : buildings) {
        if (building.IsColliding(playerX, playerY, player->getSize()) && 
            Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_SPACE)) {
            if (building.name == "anqi_house") {
                Engine::GameEngine::GetInstance().ChangeScene("start");
            }
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
