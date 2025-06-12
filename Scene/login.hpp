#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <memory>
#include "Engine/IScene.hpp"

class Login final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_FONT> font;
    std::shared_ptr<ALLEGRO_FONT> title_font;
    
    // UI 狀態
    bool show_login_ui;
    bool is_register_mode;
    
    // 輸入框
    struct InputBox {
        float x, y, width, height;
        std::string text;
        bool is_active;
        bool is_password;
        bool show_password;
    };
    
    std::vector<InputBox> input_boxes;
    int active_input_box;
    
    // 按鈕
    struct Button {
        float x, y, width, height;
        std::string text;
        bool is_hovered;
    };
    
    std::vector<Button> buttons;
    
    // 常量
    static constexpr float FONT_SIZE = 72.0f;
    static constexpr float TITLE_FONT_SIZE = 108.0f;
    static constexpr float INPUT_BOX_HEIGHT = 120.0f;
    static constexpr float BUTTON_HEIGHT = 120.0f;
    static constexpr float PADDING = 60.0f;
    static constexpr float SHOW_PASSWORD_BUTTON_SIZE = 100.0f;
    
    // 初始化 UI 元素
    void InitializeUI();
    void DrawInputBox(const InputBox& box) const;
    void DrawButton(const Button& button) const;
    void HandleInputBoxClick(float mouse_x, float mouse_y);
    void HandleButtonClick(float mouse_x, float mouse_y);
    void HandleKeyInput(int keycode);
    void SubmitForm();

public:
    Login();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keycode) override;
};

#endif // LOGIN_HPP 