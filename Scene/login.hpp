#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <memory>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
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
    
    // API 相關
    static constexpr const char* API_BASE_URL = "http://localhost:8000";
    std::string auth_token;
    
    // 返回按鈕相關
    static constexpr float BACK_BUTTON_SIZE = 100.0f;  // 返回按鈕大小
    static constexpr float BACK_BUTTON_PADDING = 30.0f;  // 返回按鈕邊距
    bool is_back_button_hovered;  // 返回按鈕懸停狀態
    std::shared_ptr<ALLEGRO_BITMAP> back_icon;  // 返回圖標
    
    // 初始化 UI 元素
    void InitializeUI();
    void DrawInputBox(const InputBox& box) const;
    void DrawButton(const Button& button) const;
    void HandleInputBoxClick(float mouse_x, float mouse_y);
    void HandleButtonClick(float mouse_x, float mouse_y);
    void HandleKeyInput(int keycode);
    void SubmitForm();
    
    // HTTP 請求相關
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    std::string MakeHttpRequest(const std::string& endpoint, const std::string& method, const nlohmann::json& data = nullptr);
    bool RegisterUser(const std::string& username, const std::string& email, const std::string& password);
    bool LoginUser(const std::string& email, const std::string& password);

    // 用戶資料結構
    struct UserData {
        std::string username;
        std::string email;
        std::string created_at;
    };
    
    // 儲存當前用戶資料
    UserData current_user;
public:
    Login();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keycode) override;
    std::string GetCurrentUserEmail() const;
    std::string GetCurrentUsername() const;
    std::string GetCurrentUserCreatedAt() const;
};

#endif // LOGIN_HPP 