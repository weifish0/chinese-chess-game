#include "setting_scene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

void SettingScene::Initialize() {
    // 載入字體
    title_font = Engine::Resources::GetInstance().GetFont("font2.ttc", TITLE_FONT_SIZE).get();
    
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/setting-bg.png").get();
    
    // 載入返回圖標
    back_icon = Engine::Resources::GetInstance().GetBitmap("back-icon.png").get();
    
    // 初始化返回按鈕狀態
    is_back_button_hovered = false;
}

void SettingScene::Terminate() {
    // 清理資源
    title_font = nullptr;
    background = nullptr;
    back_icon = nullptr;
}

void SettingScene::Update(float deltaTime) {
    // 檢查是否按下 ESC 鍵返回
    if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_ESCAPE)) {
        Engine::GameEngine::GetInstance().ChangeScene("playground");
    }
    
    // 更新返回按鈕懸停狀態
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    float mouse_x = mouse_state.x;
    float mouse_y = mouse_state.y;
    
    is_back_button_hovered = (mouse_x >= BACK_BUTTON_PADDING && 
                             mouse_x <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE &&
                             mouse_y >= BACK_BUTTON_PADDING && 
                             mouse_y <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE);
}

void SettingScene::Draw() const {
    // 繪製背景
    if (background) {
        al_draw_scaled_bitmap(
            static_cast<ALLEGRO_BITMAP*>(background),
            0, 0,
            al_get_bitmap_width(static_cast<ALLEGRO_BITMAP*>(background)),
            al_get_bitmap_height(static_cast<ALLEGRO_BITMAP*>(background)),
            0, 0,
            Engine::GameEngine::GetInstance().GetScreenWidth(),
            Engine::GameEngine::GetInstance().GetScreenHeight(),
            0
        );
    }
    
    // 繪製返回按鈕
    if (back_icon) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        float alpha = is_back_button_hovered ? 0.7f : 1.0f;
        al_draw_tinted_scaled_bitmap(
            static_cast<ALLEGRO_BITMAP*>(back_icon),
            al_map_rgba_f(alpha, alpha, alpha, 1.0f),
            0, 0,
            al_get_bitmap_width(static_cast<ALLEGRO_BITMAP*>(back_icon)),
            al_get_bitmap_height(static_cast<ALLEGRO_BITMAP*>(back_icon)),
            BACK_BUTTON_PADDING, BACK_BUTTON_PADDING,
            BACK_BUTTON_SIZE, BACK_BUTTON_SIZE,
            0
        );
    }
    
    // 繪製標題
    if (title_font) {
        al_draw_text(
            static_cast<ALLEGRO_FONT*>(title_font),
            al_map_rgb(0, 0, 0),
            Engine::GameEngine::GetInstance().GetScreenWidth() / 2,
            50,
            ALLEGRO_ALIGN_CENTER,
            "設定"
        );
    }
}

void SettingScene::OnMouseDown(int button, int mx, int my) {
    if (button != 1) return;  // 只處理左鍵點擊
    
    // 檢查是否點擊返回按鈕
    if (mx >= BACK_BUTTON_PADDING && 
        mx <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE &&
        my >= BACK_BUTTON_PADDING && 
        my <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE) {
        Engine::GameEngine::GetInstance().ChangeScene("playground");
    }
} 