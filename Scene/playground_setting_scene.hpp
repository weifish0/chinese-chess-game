#ifndef SETTING_SCENE_HPP
#define SETTING_SCENE_HPP

#include "Engine/IScene.hpp"
#include <vector>
#include <string>

class PlaygroundSettingScene final : public Engine::IScene {
public:
    PlaygroundSettingScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;

private:
    void* title_font;
    void* volume_font;
    void* background;
    void* back_icon;
    bool is_back_button_hovered;
    
    // 音量滑軌相關
    float volume;
    bool is_dragging;
    bool is_volume_slider_hovered;
    
    // 音樂切換相關
    std::vector<std::string> music_list;
    int current_music_index;
    bool is_music_button_hovered;
    
    // 字體大小
    static constexpr float TITLE_FONT_SIZE = 128.0f;
    static constexpr float VOLUME_FONT_SIZE = 80.0f;
    static constexpr float MUSIC_FONT_SIZE = 60.0f;  // 新增：音樂按鈕字體大小
    
    // 返回按鈕
    static constexpr float BACK_BUTTON_SIZE = 80.0f;  // 調整大小
    static constexpr float BACK_BUTTON_PADDING = 20.0f;
    
    // 音量控制
    static constexpr float VOLUME_SLIDER_WIDTH = 500.0f;  // 增加寬度
    static constexpr float VOLUME_SLIDER_HEIGHT = 20.0f;
    static constexpr float VOLUME_SLIDER_X = 400.0f;
    static constexpr float VOLUME_SLIDER_Y = 300.0f;
    static constexpr float VOLUME_KNOB_SIZE = 30.0f;
    static constexpr float VOLUME_LABEL_X = 200.0f;  // 音量文字位置
    static constexpr float VOLUME_LABEL_Y = 300.0f;
    
    // 音樂控制
    static constexpr float MUSIC_BUTTON_WIDTH = 500.0f;  // 增加寬度
    static constexpr float MUSIC_BUTTON_HEIGHT = 80.0f;  // 增加高度
    static constexpr float MUSIC_BUTTON_X = 400.0f;
    static constexpr float MUSIC_BUTTON_Y = 450.0f;  // 調整位置
    static constexpr float MUSIC_LABEL_X = 200.0f;  // 音樂文字位置
    static constexpr float MUSIC_LABEL_Y = 450.0f;
    
    // 間距
    static constexpr float SECTION_SPACING = 100.0f;  // 各區塊之間的間距
};

#endif // SETTING_SCENE_HPP 