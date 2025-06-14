#ifndef SETTING_SCENE_HPP
#define SETTING_SCENE_HPP

#include "Engine/IScene.hpp"

class SettingScene final : public Engine::IScene {
public:
    SettingScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;  // 新增：處理滑鼠移動

private:
    void* title_font;  // 使用 void* 來避免編譯器錯誤
    void* volume_font;  // 音量文字字體
    void* background;  // 背景圖片
    void* back_icon;   // 返回圖標
    bool is_back_button_hovered;  // 返回按鈕懸停狀態
    
    // 音量滑軌相關
    float volume;  // 當前音量值 (0.0 - 1.0)
    bool is_dragging;  // 是否正在拖動滑軌
    bool is_volume_slider_hovered;  // 滑軌是否被懸停
    
    static constexpr float TITLE_FONT_SIZE = 128.0f;
    static constexpr float VOLUME_FONT_SIZE = 80.0f;  // 音量文字字體大小
    static constexpr float BACK_BUTTON_SIZE = 100.0f;  // 返回按鈕大小
    static constexpr float BACK_BUTTON_PADDING = 30.0f;  // 返回按鈕邊距
    
    // 音量滑軌相關常量
    static constexpr float VOLUME_SLIDER_WIDTH = 400.0f;  // 滑軌寬度
    static constexpr float VOLUME_SLIDER_HEIGHT = 20.0f;  // 滑軌高度
    static constexpr float VOLUME_SLIDER_X = 400.0f;  // 滑軌 X 座標
    static constexpr float VOLUME_SLIDER_Y = 300.0f;  // 滑軌 Y 座標
    static constexpr float VOLUME_KNOB_SIZE = 30.0f;  // 滑軌按鈕大小
};

#endif // SETTING_SCENE_HPP 