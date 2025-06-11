#include "playground.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>


void Playground::Initialize() {
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/playground.png");
    
    // 載入建築圖片
    auto anqi_house_normal = Engine::Resources::GetInstance().GetBitmap("playground/anqi_house.png");
    auto anqi_house_pressed = Engine::Resources::GetInstance().GetBitmap("playground/anqi_house_pressed.png");
    auto xiangqi_house_normal = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_house.png");
    auto xiangqi_house_pressed = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_house_pressed.png");
    
    // 初始化建築物
    buildings.push_back(Building(ANQI_HOUSE_X, ANQI_HOUSE_Y, HOUSE_SIZE, HOUSE_SIZE, 
                                anqi_house_normal, anqi_house_pressed, "anqi_house"));
    buildings.push_back(Building(XIANGQI_HOUSE_X, XIANGQI_HOUSE_Y, HOUSE_SIZE, HOUSE_SIZE, 
                                xiangqi_house_normal, xiangqi_house_pressed, "xiangqi_house"));
    
    // 創建玩家
    player = new Player();
    AddNewObject(player);

    // 創建 NPC
    auto npc_image = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_NPC.png");
    NPC* npc = new NPC(700, 350, 65, npc_image);
    npc->addDialogue("你好，歡迎來到象棋世界！");
    npc->addDialogue("這裡有暗棋和象棋兩種遊戲。");
    npc->addDialogue("你可以選擇任意一個建築物進入遊戲。");
    npcs.push_back(npc);

    // 初始化鏡頭位置到玩家位置
    camera_x = player->getX() - VIEWPORT_WIDTH / 2;
    camera_y = player->getY() - VIEWPORT_HEIGHT / 2;
}

void Playground::Terminate() {
    // 清理建築物資源
    buildings.clear();
    
    // 清理 NPC 資源
    for (auto npc : npcs) {
        delete npc;
    }
    npcs.clear();
    
    // 清理其他資源
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
            building.GetCurrentImage().get(),
            0, 0, al_get_bitmap_width(building.GetCurrentImage().get()), 
            al_get_bitmap_height(building.GetCurrentImage().get()),
            draw_x, draw_y, building.width * scale_x, building.height * scale_y,
            0
        );
    }
    
    // 繪製所有 NPC
    for (const auto& npc : npcs) {
        float draw_x = (npc->getX() - camera_x - npc->getSize()/2) * scale_x;
        float draw_y = (npc->getY() - camera_y - npc->getSize()/2) * scale_y;
        npc->Draw(draw_x, draw_y, scale_x, scale_y);
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

void Playground::CheckNPCInteraction() {
    float playerX = player->getX();
    float playerY = player->getY();
    
    for (auto npc : npcs) {
        if (npc->isInRange(playerX, playerY)) {
            if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_SPACE)) {
                if (!npc->getIsTalking()) {
                    npc->startDialogue();
                } else if (npc->hasMoreDialogue()) {
                    npc->nextDialogue();
                } else {
                    npc->endDialogue();
                }
            }
        }
    }
}

void Playground::Update(float deltaTime) {
    player->Update(deltaTime);
    UpdateCamera();
    
    float playerX = player->getX();
    float playerY = player->getY();
    float playerSize = player->getSize();
    float moveSpeed = player->getSpeed();
    
    // 獲取滑鼠位置
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    int mouse_x = mouse_state.x;
    int mouse_y = mouse_state.y;
    
    // 更新建築物的懸停狀態
    float scale_x = SCREEN_RIGHT / VIEWPORT_WIDTH;
    float scale_y = SCREEN_BOTTOM / VIEWPORT_HEIGHT;
    
    // 重置所有建築物的懸停狀態
    for (auto& building : buildings) {
        building.is_hovered = false;
    }
    
    // 檢查每個建築物的懸停狀態
    for (auto& building : buildings) {
        if (building.IsMouseOver(mouse_x, mouse_y, camera_x, camera_y, scale_x, scale_y)) {
            building.is_hovered = true;
            break;
        }
    }
    
    // 檢查 NPC 互動
    CheckNPCInteraction();
    
    // 處理輸入並檢查碰撞
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_W)) {
        float newY = playerY - moveSpeed;
        if (newY - playerSize/2 > SCREEN_TOP && !CheckBuildingCollision(playerX, newY)) {
            player->moveUp();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S)) {
        float newY = playerY + moveSpeed;
        if (newY + playerSize/2 < SCREEN_BOTTOM && !CheckBuildingCollision(playerX, newY)) {
            player->moveDown();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A)) {
        float newX = playerX - moveSpeed;
        if (newX - playerSize/2 > SCREEN_LEFT && !CheckBuildingCollision(newX, playerY)) {
            player->moveLeft();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D)) {
        float newX = playerX + moveSpeed;
        if (newX + playerSize/2 < SCREEN_RIGHT && !CheckBuildingCollision(newX, playerY)) {
            player->moveRight();
        }
    }
    
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_ESCAPE)) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
}

// 新增：處理滑鼠事件
void Playground::OnMouseDown(int button, int mx, int my) {
    float scale_x = SCREEN_RIGHT / VIEWPORT_WIDTH;
    float scale_y = SCREEN_BOTTOM / VIEWPORT_HEIGHT;
    
    // 檢查是否點擊了建築物
    for (const auto& building : buildings) {
        if (building.IsMouseOver(mx, my, camera_x, camera_y, scale_x, scale_y)) {
            Engine::GameEngine::GetInstance().ChangeScene("start");
            return;
        }
    }
}
