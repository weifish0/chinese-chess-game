#include "login.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>

Login::Login() : show_login_ui(true), is_register_mode(false), active_input_box(-1) {}

void Login::Initialize() {
    // 載入資源
    font = Engine::Resources::GetInstance().GetFont("font2.ttc", FONT_SIZE);
    title_font = Engine::Resources::GetInstance().GetFont("font2.ttc", TITLE_FONT_SIZE);
    
    InitializeUI();
}

void Login::Terminate() {
    input_boxes.clear();
    buttons.clear();
}

void Login::InitializeUI() {
    float screen_width = Engine::GameEngine::GetInstance().GetScreenWidth();
    float screen_height = Engine::GameEngine::GetInstance().GetScreenHeight();
    
    // 清空現有的 UI 元素
    input_boxes.clear();
    buttons.clear();
    
    if (show_login_ui) {
        float center_x = screen_width / 2;
        float start_y = screen_height / 4;
        float input_width = 1200.0f;
        
        if (is_register_mode) {
            // 註冊模式的輸入框
            input_boxes.push_back({center_x - input_width/2, start_y, input_width, INPUT_BOX_HEIGHT, "", false, false, false}); // 用戶名
            input_boxes.push_back({center_x - input_width/2, start_y + INPUT_BOX_HEIGHT + PADDING, input_width, INPUT_BOX_HEIGHT, "", false, false, false}); // 信箱
            input_boxes.push_back({center_x - input_width/2, start_y + (INPUT_BOX_HEIGHT + PADDING) * 2, input_width, INPUT_BOX_HEIGHT, "", false, true, false}); // 密碼
            
            // 註冊模式的按鈕
            float button_width = 360.0f;
            float button_y = start_y + (INPUT_BOX_HEIGHT + PADDING) * 3 + PADDING;
            buttons.push_back({center_x - button_width/2, button_y, button_width, BUTTON_HEIGHT, "註冊", false});
            buttons.push_back({center_x - button_width/2, button_y + BUTTON_HEIGHT + PADDING, button_width, BUTTON_HEIGHT, "返回登入", false});
        } else {
            // 登入模式的輸入框
            input_boxes.push_back({center_x - input_width/2, start_y, input_width, INPUT_BOX_HEIGHT, "", false, false, false}); // 信箱
            input_boxes.push_back({center_x - input_width/2, start_y + INPUT_BOX_HEIGHT + PADDING, input_width, INPUT_BOX_HEIGHT, "", false, true, false}); // 密碼
            
            // 登入模式的按鈕
            float button_width = 360.0f;
            float button_y = start_y + (INPUT_BOX_HEIGHT + PADDING) * 2 + PADDING;
            buttons.push_back({center_x - button_width/2, button_y, button_width, BUTTON_HEIGHT, "登入", false});
            
            // 添加註冊提示文字按鈕
            float text_width = al_get_text_width(font.get(), "還沒有帳號？點我註冊");
            buttons.push_back({center_x - text_width/2, button_y + BUTTON_HEIGHT + PADDING, text_width, FONT_SIZE, "還沒有帳號？點我註冊", false});
        }
    }
}

void Login::Draw() const {
    // 繪製半透明背景
    float screen_width = Engine::GameEngine::GetInstance().GetScreenWidth();
    float screen_height = Engine::GameEngine::GetInstance().GetScreenHeight();
    
    // 清空整個畫面
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    if (show_login_ui) {
        // 繪製半透明背景
        al_draw_filled_rectangle(
            0, 0,
            screen_width, screen_height,
            al_map_rgba(0, 0, 0, 128)
        );
        
        // 繪製標題
        if (title_font) {
            const char* title = is_register_mode ? "註冊" : "登入";
            float title_width = al_get_text_width(title_font.get(), title);
            al_draw_text(
                title_font.get(),
                al_map_rgb(255, 255, 255),
                screen_width / 2,
                screen_height / 4 - TITLE_FONT_SIZE - PADDING,
                ALLEGRO_ALIGN_CENTER,
                title
            );
        }
        
        // 繪製輸入框
        for (const auto& box : input_boxes) {
            DrawInputBox(box);
        }
        
        // 繪製按鈕
        for (const auto& button : buttons) {
            DrawButton(button);
        }
    }
}

void Login::DrawInputBox(const InputBox& box) const {
    // 繪製輸入框背景
    al_draw_filled_rectangle(
        box.x, box.y,
        box.x + box.width, box.y + box.height,
        al_map_rgb(255, 255, 255)
    );
    
    // 繪製輸入框邊框
    al_draw_rectangle(
        box.x, box.y,
        box.x + box.width, box.y + box.height,
        box.is_active ? al_map_rgb(0, 120, 255) : al_map_rgb(200, 200, 200),
        2.0f
    );
    
    // 繪製標籤文字
    if (font) {
        std::string label;
        if (input_boxes.size() == 2) {  // 登入模式
            if (&box == &input_boxes[0]) {
                label = "信箱：";
            } else if (&box == &input_boxes[1]) {
                label = "密碼：";
            }
        } else if (input_boxes.size() == 3) {  // 註冊模式
            if (&box == &input_boxes[0]) {
                label = "用戶名：";
            } else if (&box == &input_boxes[1]) {
                label = "信箱：";
            } else if (&box == &input_boxes[2]) {
                label = "密碼：";
            }
        }
        
        if (!label.empty()) {
            al_draw_text(
                font.get(),
                al_map_rgb(255, 255, 255),
                box.x - 10.0f,
                box.y + (box.height - FONT_SIZE) / 2,
                ALLEGRO_ALIGN_RIGHT,
                label.c_str()
            );
        }
    }
    
    // 繪製輸入文字
    if (font) {
        std::string display_text = (box.is_password && !box.show_password) ? std::string(box.text.length(), '*') : box.text;
        al_draw_text(
            font.get(),
            al_map_rgb(0, 0, 0),
            box.x + 10.0f,
            box.y + (box.height - FONT_SIZE) / 2,
            0,
            display_text.c_str()
        );
        
        // 如果是密碼輸入框，添加顯示密碼按鈕
        if (box.is_password) {
            float button_x = box.x + box.width - SHOW_PASSWORD_BUTTON_SIZE - 10.0f;
            float button_y = box.y + (box.height - SHOW_PASSWORD_BUTTON_SIZE) / 2;
            
            // 繪製按鈕背景
            al_draw_filled_rectangle(
                button_x, button_y,
                button_x + SHOW_PASSWORD_BUTTON_SIZE, button_y + SHOW_PASSWORD_BUTTON_SIZE,
                box.show_password ? al_map_rgb(0, 120, 255) : al_map_rgb(200, 200, 200)
            );
            
            // 繪製按鈕文字
            al_draw_text(
                font.get(),
                al_map_rgb(0, 0, 0),
                button_x + SHOW_PASSWORD_BUTTON_SIZE / 2,
                button_y + (SHOW_PASSWORD_BUTTON_SIZE - FONT_SIZE) / 2,
                ALLEGRO_ALIGN_CENTER,
                box.show_password ? "隱藏" : "顯示"
            );
        }
    }
}

void Login::DrawButton(const Button& button) const {
    if (button.text == "還沒有帳號？點我註冊") {
        // 繪製文字按鈕
        if (font) {
            ALLEGRO_COLOR text_color = button.is_hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(255, 255, 255);
            al_draw_text(
                font.get(),
                text_color,
                button.x + button.width / 2,
                button.y,
                ALLEGRO_ALIGN_CENTER,
                button.text.c_str()
            );
        }
    } else {
        // 繪製一般按鈕
        al_draw_filled_rectangle(
            button.x, button.y,
            button.x + button.width, button.y + button.height,
            button.is_hovered ? al_map_rgb(0, 120, 255) : al_map_rgb(200, 200, 200)
        );
        
        // 繪製按鈕文字
        if (font) {
            al_draw_text(
                font.get(),
                al_map_rgb(0, 0, 0),
                button.x + button.width / 2,
                button.y + (button.height - FONT_SIZE) / 2,
                ALLEGRO_ALIGN_CENTER,
                button.text.c_str()
            );
        }
    }
}

void Login::Update(float deltaTime) {
    // 更新按鈕懸停狀態
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    float mouse_x = mouse_state.x;
    float mouse_y = mouse_state.y;
    
    // 重置所有按鈕的懸停狀態
    for (auto& button : buttons) {
        button.is_hovered = false;
    }
    
    // 更新按鈕懸停狀態
    for (auto& button : buttons) {
        if (mouse_x >= button.x && mouse_x <= button.x + button.width &&
            mouse_y >= button.y && mouse_y <= button.y + button.height) {
            button.is_hovered = true;
            break;  // 一次只懸停一個按鈕
        }
    }
}

void Login::OnMouseDown(int button, int mx, int my) {
    if (button != 1) return;  // 只處理左鍵點擊

    // 如果 UI 沒有顯示，不處理其他點擊
    if (!show_login_ui) return;

    // 處理輸入框點擊
    HandleInputBoxClick(mx, my);

    // 處理按鈕點擊
    HandleButtonClick(mx, my);
}

void Login::HandleInputBoxClick(float mouse_x, float mouse_y) {
    active_input_box = -1;
    for (size_t i = 0; i < input_boxes.size(); ++i) {
        auto& box = input_boxes[i];
        
        // 檢查是否點擊了顯示密碼按鈕
        if (box.is_password) {
            float button_x = box.x + box.width - SHOW_PASSWORD_BUTTON_SIZE - 10.0f;
            float button_y = box.y + (box.height - SHOW_PASSWORD_BUTTON_SIZE) / 2;
            
            if (mouse_x >= button_x && mouse_x <= button_x + SHOW_PASSWORD_BUTTON_SIZE &&
                mouse_y >= button_y && mouse_y <= button_y + SHOW_PASSWORD_BUTTON_SIZE) {
                box.show_password = !box.show_password;
                return;
            }
        }
        
        // 檢查是否點擊了輸入框
        if (mouse_x >= box.x && mouse_x <= box.x + box.width &&
            mouse_y >= box.y && mouse_y <= box.y + box.height) {
            box.is_active = true;
            active_input_box = i;
        } else {
            box.is_active = false;
        }
    }
}

void Login::HandleButtonClick(float mouse_x, float mouse_y) {
    for (const auto& button : buttons) {
        if (mouse_x >= button.x && mouse_x <= button.x + button.width &&
            mouse_y >= button.y && mouse_y <= button.y + button.height) {
            if (button.text == "註冊" || button.text == "登入") {
                SubmitForm();
            } else if (button.text == "返回登入" || button.text == "還沒有帳號？點我註冊") {
                // 清空所有輸入框和按鈕
                input_boxes.clear();
                buttons.clear();
                
                // 切換模式
                is_register_mode = !is_register_mode;
                
                // 重新初始化 UI
                InitializeUI();
            }
            break;
        }
    }
}

void Login::OnKeyDown(int keycode) {
    if (active_input_box >= 0 && active_input_box < input_boxes.size()) {
        HandleKeyInput(keycode);
    }
}

void Login::HandleKeyInput(int keycode) {
    auto& box = input_boxes[active_input_box];
    
    if (keycode == ALLEGRO_KEY_BACKSPACE) {
        if (!box.text.empty()) {
            box.text.pop_back();
        }
    } else if (keycode == ALLEGRO_KEY_ENTER) {
        SubmitForm();
    } else if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z) {
        // 處理字母輸入
        char c = 'a' + (keycode - ALLEGRO_KEY_A);
        if (Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_LSHIFT) ||
            Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_RSHIFT)) {
            c = toupper(c);
        }
        box.text += c;
    } else if(Engine::GameEngine::GetInstance().IsKeyDown(ALLEGRO_KEY_LSHIFT)){
        if (keycode == ALLEGRO_KEY_2){
            box.text += '@';
        }
    }
    else if (keycode >= ALLEGRO_KEY_0 && keycode <= ALLEGRO_KEY_9) {
        // 處理數字輸入
        box.text += '0' + (keycode - ALLEGRO_KEY_0);
    }
    else if (keycode == ALLEGRO_KEY_FULLSTOP) {
        box.text += '.';
    }
}

void Login::SubmitForm() {
    if (is_register_mode) {
        // 處理註冊
        if (input_boxes.size() >= 3) {
            std::string username = input_boxes[0].text;
            std::string email = input_boxes[1].text;
            std::string password = input_boxes[2].text;
            
            // TODO: 發送註冊請求到後端
            std::cout << "註冊: " << username << ", " << email << ", " << password << std::endl;
        }
    } else {
        // 處理登入
        if (input_boxes.size() >= 2) {
            std::string email = input_boxes[0].text;
            std::string password = input_boxes[1].text;
            
            // TODO: 發送登入請求到後端
            std::cout << "登入: " << email << ", " << password << std::endl;
        }
    }
} 