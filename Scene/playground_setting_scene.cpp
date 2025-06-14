#include "playground_setting_scene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "playground.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

void PlaygroundSettingScene::Initialize() {
    // 載入字體
    title_font = Engine::Resources::GetInstance().GetFont("font2.ttc", TITLE_FONT_SIZE).get();
    volume_font = Engine::Resources::GetInstance().GetFont("font2.ttc", VOLUME_FONT_SIZE).get();
    
    // 載入背景圖片
    background = Engine::Resources::GetInstance().GetBitmap("playground/playground_setting-bg.png").get();
    
    // 載入返回圖標
    back_icon = Engine::Resources::GetInstance().GetBitmap("back-icon.png").get();
    
    // 初始化返回按鈕狀態
    is_back_button_hovered = false;
    
    // 初始化音量滑軌
    volume = 1.0f;
    is_dragging = false;
    is_volume_slider_hovered = false;
    
    // 初始化音樂列表
    music_list = {
        "playground/A Tender Feeling.ogg",
        "playground/Undertale-OST-071.ogg",
        "playground/Stardew-Valley-OST-Summer.ogg"
    };
    
    // 從 Playground 場景獲取當前音樂索引
    auto playground = dynamic_cast<Playground*>(Engine::GameEngine::GetInstance().GetScene("playground"));
    if (playground) {
        current_music_index = playground->GetCurrentMusicIndex();
    } else {
        current_music_index = 0;
    }
    
    is_music_button_hovered = false;
}

void PlaygroundSettingScene::Terminate() {
    // 清理資源
    title_font = nullptr;
    volume_font = nullptr;
    background = nullptr;
    back_icon = nullptr;
}

void PlaygroundSettingScene::Update(float deltaTime) {
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
    
    // 更新音量滑軌懸停狀態
    is_volume_slider_hovered = (mouse_x >= VOLUME_SLIDER_X - VOLUME_KNOB_SIZE/2 && 
                               mouse_x <= VOLUME_SLIDER_X + VOLUME_SLIDER_WIDTH + VOLUME_KNOB_SIZE/2 &&
                               mouse_y >= VOLUME_SLIDER_Y - VOLUME_KNOB_SIZE/2 && 
                               mouse_y <= VOLUME_SLIDER_Y + VOLUME_KNOB_SIZE/2);
    
    // 更新音樂切換按鈕懸停狀態
    is_music_button_hovered = (mouse_x >= MUSIC_BUTTON_X && 
                              mouse_x <= MUSIC_BUTTON_X + MUSIC_BUTTON_WIDTH &&
                              mouse_y >= MUSIC_BUTTON_Y + 100 && 
                              mouse_y <= MUSIC_BUTTON_Y + MUSIC_BUTTON_HEIGHT + 100);
    
    // 如果正在拖動滑軌且滑鼠在滑軌區域內，更新音量值
    if (is_dragging && is_volume_slider_hovered) {
        float knob_x = std::max(VOLUME_SLIDER_X, 
                      std::min(VOLUME_SLIDER_X + VOLUME_SLIDER_WIDTH, 
                      static_cast<float>(mouse_x)));
        volume = (knob_x - VOLUME_SLIDER_X) / VOLUME_SLIDER_WIDTH;
        
        // 更新全局音量
        al_set_mixer_gain(al_get_default_mixer(), volume);
    }
}

void PlaygroundSettingScene::Draw() const {
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
    
    // 繪製音量控制區
    // 繪製音量標籤
    if (volume_font) {
        al_draw_text(
            static_cast<ALLEGRO_FONT*>(volume_font),
            al_map_rgb(0, 0, 0),
            VOLUME_LABEL_X,
            VOLUME_LABEL_Y - al_get_font_line_height(static_cast<ALLEGRO_FONT*>(volume_font))/2,
            ALLEGRO_ALIGN_LEFT,
            "音量"
        );
    }
    
    // 繪製音量滑軌背景
    al_draw_filled_rounded_rectangle(
        VOLUME_SLIDER_X, VOLUME_SLIDER_Y,
        VOLUME_SLIDER_X + VOLUME_SLIDER_WIDTH, VOLUME_SLIDER_Y + VOLUME_SLIDER_HEIGHT,
        VOLUME_SLIDER_HEIGHT/2, VOLUME_SLIDER_HEIGHT/2,
        al_map_rgb(200, 200, 200)
    );
    
    // 繪製音量滑軌按鈕
    float knob_x = VOLUME_SLIDER_X + volume * VOLUME_SLIDER_WIDTH;
    al_draw_filled_circle(
        knob_x, VOLUME_SLIDER_Y + VOLUME_SLIDER_HEIGHT/2,
        VOLUME_KNOB_SIZE/2,
        is_volume_slider_hovered || is_dragging ? al_map_rgb(100, 100, 100) : al_map_rgb(150, 150, 150)
    );
    
    // 繪製音樂控制區
    // 繪製音樂標籤
    std::string music_label = "音樂： " + music_list[current_music_index].substr(music_list[current_music_index].find_last_of("/"));
    if (volume_font) {
        al_draw_text(
            static_cast<ALLEGRO_FONT*>(volume_font),
            al_map_rgb(0, 0, 0),
            MUSIC_LABEL_X,
            MUSIC_LABEL_Y - al_get_font_line_height(static_cast<ALLEGRO_FONT*>(volume_font))/2,
            ALLEGRO_ALIGN_LEFT,
            music_label.c_str()
        );
    }
    
    // 繪製音樂切換按鈕
    al_draw_filled_rounded_rectangle(
        MUSIC_BUTTON_X, MUSIC_BUTTON_Y + 100,
        MUSIC_BUTTON_X + MUSIC_BUTTON_WIDTH, MUSIC_BUTTON_Y + MUSIC_BUTTON_HEIGHT + 100, 
        10, 10,
        is_music_button_hovered ? al_map_rgb(180, 180, 180) : al_map_rgb(200, 200, 200)
    );
    
    // 繪製音樂切換按鈕文字
    if (volume_font) {
        std::string button_text = "切換音樂";
        al_draw_text(
            static_cast<ALLEGRO_FONT*>(volume_font),
            al_map_rgb(0, 0, 0),
            MUSIC_BUTTON_X + MUSIC_BUTTON_WIDTH/2,
            MUSIC_BUTTON_Y + 100,
            ALLEGRO_ALIGN_CENTER,
            button_text.c_str()
        );
    }
}

void PlaygroundSettingScene::OnMouseDown(int button, int mx, int my) {
    if (button != 1) return;  // 只處理左鍵點擊
    
    // 檢查是否點擊返回按鈕
    if (mx >= BACK_BUTTON_PADDING && 
        mx <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE &&
        my >= BACK_BUTTON_PADDING && 
        my <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE) {
        Engine::GameEngine::GetInstance().ChangeScene("playground");
        return;
    }
    
    // 檢查是否點擊音量滑軌
    if (is_volume_slider_hovered) {
        is_dragging = true;
        // 立即更新音量值
        float knob_x = std::max(VOLUME_SLIDER_X, 
                      std::min(VOLUME_SLIDER_X + VOLUME_SLIDER_WIDTH, 
                      static_cast<float>(mx)));
        volume = (knob_x - VOLUME_SLIDER_X) / VOLUME_SLIDER_WIDTH;
        al_set_mixer_gain(al_get_default_mixer(), volume);
    }
    
    // 檢查是否點擊音樂切換按鈕
    if (is_music_button_hovered) {
        // 切換到下一首音樂
        current_music_index = (current_music_index + 1) % music_list.size();
        
        // 獲取當前場景
        auto playground = dynamic_cast<Playground*>(Engine::GameEngine::GetInstance().GetScene("playground"));
        if (playground) {
            // 停止當前音樂
            if (playground->GetMusicInstance()) {
                al_stop_sample_instance(playground->GetMusicInstance().get());
            }
            
            // 載入並播放新音樂
            auto new_music = Engine::Resources::GetInstance().GetSample(music_list[current_music_index]);
            if (new_music) {
                auto new_instance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>(
                    al_create_sample_instance(new_music.get()),
                    al_destroy_sample_instance
                );
                if (new_instance) {
                    al_attach_sample_instance_to_mixer(new_instance.get(), al_get_default_mixer());
                    al_set_sample_instance_playmode(new_instance.get(), ALLEGRO_PLAYMODE_LOOP);
                    al_play_sample_instance(new_instance.get());
                    playground->SetMusicInstance(new_instance);
                    playground->SetCurrentMusicIndex(current_music_index);  // 更新 Playground 中的音樂索引
                }
            }
        }
    }
}

void PlaygroundSettingScene::OnMouseMove(int mx, int my) {
    // 更新音量滑軌懸停狀態
    is_volume_slider_hovered = (mx >= VOLUME_SLIDER_X - VOLUME_KNOB_SIZE/2 && 
                               mx <= VOLUME_SLIDER_X + VOLUME_SLIDER_WIDTH + VOLUME_KNOB_SIZE/2 &&
                               my >= VOLUME_SLIDER_Y - VOLUME_KNOB_SIZE/2 && 
                               my <= VOLUME_SLIDER_Y + VOLUME_KNOB_SIZE/2);
    
    // 如果正在拖動滑軌且滑鼠在滑軌區域內，更新音量值
    if (is_dragging && is_volume_slider_hovered) {
        float knob_x = std::max(VOLUME_SLIDER_X, 
                      std::min(VOLUME_SLIDER_X + VOLUME_SLIDER_WIDTH, 
                      static_cast<float>(mx)));
        volume = (knob_x - VOLUME_SLIDER_X) / VOLUME_SLIDER_WIDTH;
        
        // 更新全局音量
        al_set_mixer_gain(al_get_default_mixer(), volume);
    }
    
    // 如果滑鼠離開滑軌區域，停止拖動
    if (is_dragging && !is_volume_slider_hovered) {
        is_dragging = false;
    }
} 