#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>
#include <cmath>
#include "Engine/IScene.hpp"
#include "Player/player.hpp"
#include "NPC/npc.hpp"
#include "login.hpp"

// 建築物結構
struct Building {
    float x;
    float y;
    float width;
    float height;
    std::shared_ptr<ALLEGRO_BITMAP> normal_image;    // 一般狀態圖片
    std::shared_ptr<ALLEGRO_BITMAP> pressed_image;   // 按下狀態圖片
    std::string name;
    bool is_hovered;  // 滑鼠是否懸停在建築上

    Building(float x, float y, float width, float height, 
            std::shared_ptr<ALLEGRO_BITMAP> normal_img,
            std::shared_ptr<ALLEGRO_BITMAP> pressed_img,
            std::string name)
        : x(x), y(y), width(width), height(height), 
          normal_image(normal_img), pressed_image(pressed_img),
          name(name), is_hovered(false) {}

    bool IsColliding(float playerX, float playerY, float playerSize) const {
        // 檢查玩家是否與建築物碰撞
        return (playerX + playerSize/2 > x - width/2 &&
                playerX - playerSize/2 < x + width/2 &&
                playerY + playerSize/2 > y - height/2 &&
                playerY - playerSize/2 < y + height/2);
    }

    bool IsMouseOver(float mouseX, float mouseY, float camera_x, float camera_y, float scale_x, float scale_y) const {
        // 將滑鼠座標轉換為遊戲世界座標
        float world_mouse_x = mouseX / scale_x + camera_x;
        float world_mouse_y = mouseY / scale_y + camera_y;

        return (world_mouse_x > x - width/2 &&
                world_mouse_x < x + width/2 &&
                world_mouse_y > y - height/2 &&
                world_mouse_y < y + height/2);
    }

    std::shared_ptr<ALLEGRO_BITMAP> GetCurrentImage() const {
        return is_hovered ? pressed_image : normal_image;
    }
};

class Playground final : public Engine::IScene {
private:
    Player* player;
    std::shared_ptr<ALLEGRO_BITMAP> background;  // 背景圖片
    std::shared_ptr<ALLEGRO_BITMAP> anqi_house;  // 暗棋建築圖片
    std::shared_ptr<ALLEGRO_BITMAP> xiangqi_house;  // 象棋建築圖片
    std::vector<Building> buildings;  // 建築物列表
    std::vector<NPC*> npcs;  // 新增：NPC 列表
    float camera_x;  // 鏡頭位置 X
    float camera_y;  // 鏡頭位置 Y
    bool space_key_was_down;  // 新增：追蹤空白鍵的狀態
    std::shared_ptr<ALLEGRO_FONT> name_font;  // 一般名稱字體
    std::shared_ptr<ALLEGRO_FONT> building_font;  // 建築物名稱字體
    std::shared_ptr<ALLEGRO_FONT> dialogue_font;  // 對話框字體
    std::shared_ptr<ALLEGRO_BITMAP> login_icon;  // 登入圖標
    bool is_login_icon_hovered;  // 登入圖標懸停狀態
    std::shared_ptr<ALLEGRO_BITMAP> settings_icon;  // 設定圖標
    bool is_settings_icon_hovered;  // 設定圖標懸停狀態
    std::shared_ptr<ALLEGRO_SAMPLE> background_music;  // 背景音樂
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> music_instance;  // 音樂實例
    
    // 保存玩家位置
    static float saved_player_x;
    static float saved_player_y;
    static bool has_saved_position;

    // 螢幕邊界
    static constexpr float SCREEN_LEFT = 0;
    static constexpr float SCREEN_RIGHT = 2666; 
    static constexpr float SCREEN_TOP = 0;
    static constexpr float SCREEN_BOTTOM = 1500;

    // 鏡頭相關
    static constexpr float VIEWPORT_WIDTH = 800;   // 視窗寬度
    static constexpr float VIEWPORT_HEIGHT = 600;  // 視窗高度

    // 字體相關
    static constexpr float NAME_FONT_SIZE = 40.0f;  // 一般名稱字體大小
    static constexpr float BUILDING_FONT_SIZE = 80.0f;  // 建築物名稱字體大小
    static constexpr float DIALOGUE_FONT_SIZE = 60.0f;  // 對話框字體大小

    // 建築位置
    static constexpr float ANQI_HOUSE_X = 600;
    static constexpr float ANQI_HOUSE_Y = 1000;
    static constexpr float XIANGQI_HOUSE_X = 1600;
    static constexpr float XIANGQI_HOUSE_Y = 200;
    static constexpr float HOUSE_SIZE = 300;

    // 對話框相關
    static constexpr float DIALOGUE_BOX_HEIGHT = 350.0f;  // 對話框高度
    static constexpr float DIALOGUE_PADDING = 20.0f;  // 對話框內邊距

    // 登入圖標相關
    static constexpr float ICON_SIZE = 100.0f;  // 登入圖標大小
    static constexpr float PADDING = 20.0f;  // 邊距

    // 小地圖相關
    static constexpr float MINIMAP_SIZE = 450.0f;  // 小地圖大小（原來的 3 倍）
    static constexpr float MINIMAP_PADDING = 20.0f;  // 小地圖邊距
    static constexpr float MINIMAP_SCALE = MINIMAP_SIZE / SCREEN_RIGHT;  // 小地圖縮放比例
    static constexpr float MINIMAP_PLAYER_SIZE = 12.0f;  // 玩家在小地圖上的大小（原來的 3 倍）
    static constexpr float MINIMAP_BUILDING_SIZE = 18.0f;  // 建築在小地圖上的大小（原來的 3 倍）
    static bool show_minimap;  // 控制小地圖顯示狀態
    static bool m_key_was_down;  // 追蹤 M 鍵的狀態

    // 音樂相關
    int current_music_index = 0;  // 新增：當前音樂索引

public:
    explicit Playground() : camera_x(0), camera_y(0), space_key_was_down(false) {}
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;  // 覆寫 Draw 方法來繪製背景

    // 滑鼠點擊事件
    void OnMouseDown(int button, int mx, int my) override;

    // 獲取音樂實例
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> GetMusicInstance() const { return music_instance; }
    // 設置音樂實例
    void SetMusicInstance(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> instance) { music_instance = instance; }
    // 獲取當前音樂索引
    int GetCurrentMusicIndex() const { return current_music_index; }
    // 設置當前音樂索引
    void SetCurrentMusicIndex(int index) { current_music_index = index; }

private:
    // 更新鏡頭位置
    void UpdateCamera();
    bool CheckBuildingCollision(float newX, float newY);  // 新增：檢查建築物碰撞
    bool CheckNPCCollision(float newX, float newY);  // 新增：檢查 NPC 碰撞
    void CheckNPCInteraction();  // 新增：檢查 NPC 互動
};

#endif // PLAYGROUND_HPP 