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

private:
    void* title_font;  // 使用 void* 來避免編譯器錯誤
    void* background;  // 背景圖片
    void* back_icon;   // 返回圖標
    bool is_back_button_hovered;  // 返回按鈕懸停狀態
    
    static constexpr float TITLE_FONT_SIZE = 128.0f;
    static constexpr float BACK_BUTTON_SIZE = 100.0f;  // 返回按鈕大小
    static constexpr float BACK_BUTTON_PADDING = 30.0f;  // 返回按鈕邊距
};

#endif // SETTING_SCENE_HPP 