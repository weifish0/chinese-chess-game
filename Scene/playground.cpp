#include "playground.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>


void Playground::Initialize() {
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/playground.png");
    
    // 載入字體
    name_font = Engine::Resources::GetInstance().GetFont("font2.ttc", NAME_FONT_SIZE);
    building_font = Engine::Resources::GetInstance().GetFont("font2.ttc", BUILDING_FONT_SIZE);
    dialogue_font = Engine::Resources::GetInstance().GetFont("font2.ttc", DIALOGUE_FONT_SIZE);
    
    // 載入建築圖片
    auto anqi_house_normal = Engine::Resources::GetInstance().GetBitmap("playground/anqi_house.png");
    auto anqi_house_pressed = Engine::Resources::GetInstance().GetBitmap("playground/anqi_house_pressed.png");
    auto xiangqi_house_normal = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_house.png");
    auto xiangqi_house_pressed = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_house_pressed.png");
    
    // 初始化建築物
    buildings.push_back(Building(ANQI_HOUSE_X, ANQI_HOUSE_Y, HOUSE_SIZE, HOUSE_SIZE, 
                                anqi_house_normal, anqi_house_pressed, "暗棋館"));
    buildings.push_back(Building(XIANGQI_HOUSE_X, XIANGQI_HOUSE_Y, HOUSE_SIZE, HOUSE_SIZE, 
                                xiangqi_house_normal, xiangqi_house_pressed, "象棋館"));
    
    // 創建玩家
    player = new Player();
    AddNewObject(player);

    // 創建 NPC
    auto npc1_image = Engine::Resources::GetInstance().GetBitmap("playground/mr_shen_NPC.png");
    auto npc1_dialogue_image = Engine::Resources::GetInstance().GetBitmap("playground/mr_shen_NPC_dialogue.png");
    NPC* npc1 = new NPC(700, 350, 65, npc1_image, "沈大師", npc1_dialogue_image);
    npc1->addDialogue("Hello, welcome to the chess world!");
    npc1->addDialogue("There are two games here: anqi and xiangqi.");
    npc1->addDialogue("Please choose a building to enter the game.");
    npcs.push_back(npc1);

    auto npc2_image = Engine::Resources::GetInstance().GetBitmap("playground/anqi_NPC.png");
    auto npc2_dialogue_image = Engine::Resources::GetInstance().GetBitmap("playground/anqi_NPC_dialogue.png");
    NPC* npc2 = new NPC(800, 900, 65, npc2_image, "暗棋高手", npc2_dialogue_image);
    npc2->addDialogue("Hello, welcome to the chess world!");
    npc2->addDialogue("There are two games here: anqi and xiangqi.");
    npc2->addDialogue("Please choose a building to enter the game.");
    npcs.push_back(npc2);

    auto npc3_image = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_NPC.png");
    auto npc3_dialogue_image = Engine::Resources::GetInstance().GetBitmap("playground/xiangqi_NPC_dialogue.png");
    NPC* npc3 = new NPC(1450, 350, 65, npc3_image, "象棋高手", npc3_dialogue_image);
    npc3->addDialogue("Hello, welcome to the chess world!");
    npc3->addDialogue("There are two games here: anqi and xiangqi.");
    npc3->addDialogue("Please choose a building to enter the game.");
    npcs.push_back(npc3);

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
        
        // 繪製建築物名稱
        if (building_font) {
            float text_width = al_get_text_width(building_font.get(), building.name.c_str());
            float text_height = al_get_font_line_height(building_font.get());
            float padding = 15.0f;
            
            al_draw_filled_rectangle(
                draw_x + building.width * scale_x / 2 - text_width/2 - padding,
                draw_y - text_height - 10 - padding,
                draw_x + building.width * scale_x / 2 + text_width/2 + padding,
                draw_y - 10 + padding,
                al_map_rgba(0, 0, 0, 180)
            );
            
            al_draw_text(
                building_font.get(),
                al_map_rgb(255, 255, 255),
                draw_x + building.width * scale_x / 2,
                draw_y - text_height - 10,
                ALLEGRO_ALIGN_CENTER,
                building.name.c_str()
            );
        }
    }
    
    // 繪製所有 NPC
    for (const auto& npc : npcs) {
        float draw_x = (npc->getX() - camera_x - npc->getSize()/2) * scale_x;
        float draw_y = (npc->getY() - camera_y - npc->getSize()/2) * scale_y;
        npc->Draw(draw_x, draw_y, scale_x, scale_y);
        
        // 繪製 NPC 名稱
        if (name_font) {
            float text_width = al_get_text_width(name_font.get(), npc->getName().c_str());
            float text_height = al_get_font_line_height(name_font.get());
            float padding = 10.0f;
            
            al_draw_filled_rectangle(
                draw_x + npc->getSize() * scale_x / 2 - text_width/2 - padding - 20,
                draw_y - text_height - 10 - padding,
                draw_x + npc->getSize() * scale_x / 2 + text_width/2 + padding - 20,
                draw_y - 10 + padding,
                al_map_rgba(0, 0, 0, 180)
            );
            
            al_draw_text(
                name_font.get(),
                al_map_rgb(255, 255, 255),
                draw_x + npc->getSize() * scale_x / 2 - 20,
                draw_y - text_height - 10,
                ALLEGRO_ALIGN_CENTER,
                npc->getName().c_str()
            );
        }
    }
    
    // 繪製玩家
    if (player) {
        float sprite_width = player->getSize();
        float sprite_height = player->getSize();
        float draw_x = (player->getX() - camera_x - sprite_width / 2) * scale_x;
        float draw_y = (player->getY() - camera_y - sprite_height / 2) * scale_y;
        player->Draw(draw_x, draw_y, scale_x, scale_y);
        
        // 繪製玩家名稱
        if (name_font) {
            float text_width = al_get_text_width(name_font.get(), "玩家");
            float text_height = al_get_font_line_height(name_font.get());
            float padding = 10.0f;
            
            al_draw_filled_rectangle(
                draw_x + sprite_width * scale_x / 2 - text_width/2 - padding + 30,
                draw_y - text_height - 10 - padding,
                draw_x + sprite_width * scale_x / 2 + text_width/2 + padding + 30,
                draw_y - 10 + padding,
                al_map_rgba(0, 0, 0, 180)
            );
            
            al_draw_text(
                name_font.get(),
                al_map_rgb(255, 255, 255),
                draw_x + sprite_width * scale_x / 2 + 30,
                draw_y - text_height - 10,
                ALLEGRO_ALIGN_CENTER,
                "玩家"
            );
        }
    }

    // 繪製對話框
    for (const auto& npc : npcs) {
        if (npc->getIsTalking() && dialogue_font) {
            std::string current_dialogue = npc->getCurrentDialogue();
            if (!current_dialogue.empty()) {
                // 繪製 NPC 對話圖片
                auto dialogue_image = npc->getDialogueImage();
                if (dialogue_image) {
                    float image_width = al_get_bitmap_width(dialogue_image.get());
                    float image_height = al_get_bitmap_height(dialogue_image.get());
                    float image_x = 0;
                    float image_y = 50;
                    
                    al_draw_scaled_bitmap(
                        dialogue_image.get(),
                        0, 0,
                        al_get_bitmap_width(dialogue_image.get()),
                        al_get_bitmap_height(dialogue_image.get()),
                        image_x, image_y,
                        image_width, image_height,
                        0
                    );
                }

                // 繪製對話框背景
                float box_y = SCREEN_BOTTOM - DIALOGUE_BOX_HEIGHT;
                al_draw_filled_rectangle(
                    0, box_y,
                    SCREEN_RIGHT, SCREEN_BOTTOM,
                    al_map_rgb(210, 180, 140)  // 棕色
                );

                // 繪製對話框邊框
                al_draw_rectangle(
                    0, box_y,
                    SCREEN_RIGHT, SCREEN_BOTTOM,
                    al_map_rgb(0, 0, 0),  // 黑色邊框
                    2.0f
                );

                // 繪製說話者名稱
                float name_y = box_y + DIALOGUE_PADDING;
                al_draw_text(
                    dialogue_font.get(),
                    al_map_rgb(0, 0, 0),
                    DIALOGUE_PADDING,
                    name_y,
                    ALLEGRO_ALIGN_LEFT,
                    (npc->getName() + "：").c_str()
                );

                // 繪製對話內容
                float text_y = name_y + al_get_font_line_height(dialogue_font.get()) + DIALOGUE_PADDING;
                al_draw_text(
                    dialogue_font.get(),
                    al_map_rgb(0, 0, 0),  // 黑色文字
                    DIALOGUE_PADDING,
                    text_y,
                    ALLEGRO_ALIGN_LEFT,
                    current_dialogue.c_str()
                );
            }
        }
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

bool Playground::CheckNPCCollision(float newX, float newY) {
    for (const auto& npc : npcs) {
        if (npc->IsColliding(newX, newY, player->getSize())) {
            return true;
        }
    }
    return false;
}

void Playground::CheckNPCInteraction() {
    // 檢查是否在 NPC 互動範圍內
    for (auto npc : npcs) {
        if (npc->isInRange(player->getX(), player->getY())) {
            // 檢查空白鍵的狀態變化
            bool space_key_is_down = Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_SPACE);
            if (space_key_is_down && !space_key_was_down) {  // 只在按下的那一瞬間觸發
                if (!npc->getIsTalking()) {
                    // 開始對話
                    npc->startDialogue();
                } else if (npc->hasMoreDialogue()) {
                    // 下一句對話
                    npc->nextDialogue();
                } else {
                    // 結束對話
                    npc->endDialogue();
                }
            }
            space_key_was_down = space_key_is_down;  // 更新狀態
            break;  // 一次只處理一個 NPC
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
        if (newY - playerSize/2 > SCREEN_TOP && 
            !CheckBuildingCollision(playerX, newY) && 
            !CheckNPCCollision(playerX, newY)) {
            player->moveUp();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_S)) {
        float newY = playerY + moveSpeed;
        if (newY + playerSize/2 < SCREEN_BOTTOM && 
            !CheckBuildingCollision(playerX, newY) && 
            !CheckNPCCollision(playerX, newY)) {
            player->moveDown();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_A)) {
        float newX = playerX - moveSpeed;
        if (newX - playerSize/2 > SCREEN_LEFT && 
            !CheckBuildingCollision(newX, playerY) && 
            !CheckNPCCollision(newX, playerY)) {
            player->moveLeft();
        }
    }
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_D)) {
        float newX = playerX + moveSpeed;
        if (newX + playerSize/2 < SCREEN_RIGHT && 
            !CheckBuildingCollision(newX, playerY) && 
            !CheckNPCCollision(newX, playerY)) {
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
        if (building.IsMouseOver(mx, my, camera_x, camera_y, scale_x, scale_y) && building.name == "暗棋館") {
            Engine::GameEngine::GetInstance().ChangeScene("anqi_start");
            return;
        }
        if (building.IsMouseOver(mx, my, camera_x, camera_y, scale_x, scale_y) && building.name == "象棋館") {
            Engine::GameEngine::GetInstance().ChangeScene("xiangqi");
            return;
        }
    }
}
