#include "login.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <iostream>
#include <sstream>

Login::Login() : show_login_ui(true), is_register_mode(false), active_input_box(-1), show_error(false) {
    // 初始化 CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void Login::Initialize() {
    // 載入資源
    font = Engine::Resources::GetInstance().GetFont("font2.ttc", FONT_SIZE);
    title_font = Engine::Resources::GetInstance().GetFont("font2.ttc", TITLE_FONT_SIZE);
    back_icon = Engine::Resources::GetInstance().GetBitmap("back-icon.png");  // 載入返回圖標
    
    InitializeUI();
}

void Login::Terminate() {
    input_boxes.clear();
    buttons.clear();
    // 清理 CURL
    curl_global_cleanup();
}

void Login::InitializeUI() {
    float screen_width = Engine::GameEngine::GetInstance().GetScreenWidth();
    float screen_height = Engine::GameEngine::GetInstance().GetScreenHeight();
    
    // 清空現有的 UI 元素
    input_boxes.clear();
    buttons.clear();
    
    // 添加返回按鈕
    buttons.push_back({
        BACK_BUTTON_PADDING,  // x
        BACK_BUTTON_PADDING,  // y
        BACK_BUTTON_SIZE,     // width
        BACK_BUTTON_SIZE,     // height
        "",                   // text (不需要文字)
        false                 // is_hovered
    });
    
    if (show_login_ui) {
        float center_x = screen_width / 2;
        float start_y = screen_height / 4;
        float input_width = 1200.0f;
        
        // 如果已登入，添加登出按鈕
        if (!current_user.username.empty()) {
            // 計算資料框的高度
            float box_height = FONT_SIZE * 4 + PADDING * 2;
            float logout_y = start_y + box_height + PADDING * 2;  // 在資料框下方留出更多空間
            
            // 添加登出按鈕
            buttons.push_back({
                center_x - LOGOUT_BUTTON_WIDTH / 2,
                logout_y,
                LOGOUT_BUTTON_WIDTH,
                BUTTON_HEIGHT,
                "登出",
                false
            });
            return;
        }
        
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

// 添加日期格式轉換函數
std::string Login::FormatDate(const std::string& iso_date) const{
    try {
        // 解析 ISO 格式的日期字串 (例如: "2024-03-13T10:50:00")
        std::tm tm = {};
        std::istringstream ss(iso_date);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        
        if (ss.fail()) {
            return iso_date;  // 如果解析失敗，返回原始字串
        }
        
        // 轉換為指定格式
        std::ostringstream result;
        result << (tm.tm_year + 1900) << "年"
               << (tm.tm_mon + 1) << "月"
               << tm.tm_mday << "日"
               << tm.tm_hour+8 << "點"
               << tm.tm_min << "分";
        
        return result.str();
    } catch (const std::exception& e) {
        return iso_date;  // 如果發生任何錯誤，返回原始字串
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
        
        // 檢查是否已登入
        if (!current_user.username.empty()) {
            // 已登入，顯示用戶資料
            if (title_font) {
                // 繪製標題
                al_draw_text(
                    title_font.get(),
                    al_map_rgb(255, 255, 255),
                    screen_width / 2,
                    screen_height / 4 - TITLE_FONT_SIZE - PADDING,
                    ALLEGRO_ALIGN_CENTER,
                    "帳號資料"
                );
            }
            
            if (font) {
                // 計算文字寬度
                float text_width = std::max({
                    al_get_text_width(font.get(), ("用戶名：" + current_user.username).c_str()),
                    al_get_text_width(font.get(), ("電子郵件：" + current_user.email).c_str()),
                    al_get_text_width(font.get(), ("創建時間：" + FormatDate(current_user.created_at)).c_str())
                });
                
                // 繪製背景
                float box_x = screen_width / 2 - text_width / 2 - PADDING;
                float box_y = screen_height / 4;
                float box_width = text_width + PADDING * 2;
                float box_height = FONT_SIZE * 4 + PADDING * 2;
                
                al_draw_filled_rectangle(
                    box_x, box_y,
                    box_x + box_width, box_y + box_height,
                    al_map_rgba(0, 0, 0, 180)
                );
                
                // 繪製用戶資料
                float text_y = box_y + PADDING;
                al_draw_text(
                    font.get(),
                    al_map_rgb(255, 255, 255),
                    screen_width / 2,
                    text_y,
                    ALLEGRO_ALIGN_CENTER,
                    ("使用者名稱：" + current_user.username).c_str()
                );
                
                text_y += FONT_SIZE + PADDING;
                al_draw_text(
                    font.get(),
                    al_map_rgb(255, 255, 255),
                    screen_width / 2,
                    text_y,
                    ALLEGRO_ALIGN_CENTER,
                    ("電子郵件：" + current_user.email).c_str()
                );
                
                text_y += FONT_SIZE + PADDING;
                al_draw_text(
                    font.get(),
                    al_map_rgb(255, 255, 255),
                    screen_width / 2,
                    text_y,
                    ALLEGRO_ALIGN_CENTER,
                    ("創建時間：" + FormatDate(current_user.created_at)).c_str()
                );
            }
        } else {
            // 未登入，顯示登入/註冊介面
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
        }
        
        // 在輸入框下方顯示錯誤訊息
        if (show_error && font) {
            float error_y = 50;
            
            float text_width = al_get_text_width(font.get(), error_message.c_str());
            float box_x = screen_width / 2 - text_width / 2 - PADDING;
            float box_y = error_y;
            float box_width = text_width + PADDING * 2;
            float box_height = FONT_SIZE + PADDING;
            
            al_draw_filled_rectangle(
                box_x, box_y,
                box_x + box_width, box_y + box_height,
                al_map_rgba(255, 0, 0, 180)  // 紅色半透明背景
            );
            
            al_draw_text(
                font.get(),
                al_map_rgb(255, 255, 255),  // 白色文字
                screen_width / 2,
                box_y + PADDING / 2,
                ALLEGRO_ALIGN_CENTER,
                error_message.c_str()
            );
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
    // 檢查是否為返回按鈕
    if (button.x == BACK_BUTTON_PADDING && 
        button.y == BACK_BUTTON_PADDING && 
        button.width == BACK_BUTTON_SIZE && 
        button.height == BACK_BUTTON_SIZE) {
        // 返回按鈕的繪製
        if (back_icon) {
            al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
            float alpha = button.is_hovered ? 0.7f : 1.0f;
            al_draw_tinted_scaled_bitmap(
                back_icon.get(),
                al_map_rgba_f(alpha, alpha, alpha, 1.0f),
                0, 0,
                al_get_bitmap_width(back_icon.get()),
                al_get_bitmap_height(back_icon.get()),
                button.x, button.y,
                BACK_BUTTON_SIZE, BACK_BUTTON_SIZE,
                0
            );
        }
        return;
    }
    
    if (current_user.username.empty()) {
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
    } else {
        // 已登入狀態下的按鈕繪製
        if (button.text == "登出") {
            // 繪製登出按鈕（紅色系）
            al_draw_filled_rectangle(
                button.x, button.y,
                button.x + button.width, button.y + button.height,
                button.is_hovered ? al_map_rgb(200, 50, 50) : al_map_rgb(150, 50, 50)
            );
            
            // 繪製按鈕文字
            if (font) {
                al_draw_text(
                    font.get(),
                    al_map_rgb(255, 255, 255),  // 白色文字
                    button.x + button.width / 2,
                    button.y + (button.height - FONT_SIZE) / 2,
                    ALLEGRO_ALIGN_CENTER,
                    button.text.c_str()
                );
            }
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
    
    // 更新返回按鈕懸停狀態
    if (mouse_x >= BACK_BUTTON_PADDING && 
        mouse_x <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE &&
        mouse_y >= BACK_BUTTON_PADDING && 
        mouse_y <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE) {
        buttons[0].is_hovered = true;  // 返回按鈕是第一個按鈕
    }
    
    // 更新其他按鈕懸停狀態
    for (size_t i = 1; i < buttons.size(); ++i) {  // 從第二個按鈕開始
        auto& button = buttons[i];
        if (mouse_x >= button.x && mouse_x <= button.x + button.width &&
            mouse_y >= button.y && mouse_y <= button.y + button.height) {
            button.is_hovered = true;
            break;
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
    // 檢查是否點擊返回按鈕
    if (mouse_x >= BACK_BUTTON_PADDING && 
        mouse_x <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE &&
        mouse_y >= BACK_BUTTON_PADDING && 
        mouse_y <= BACK_BUTTON_PADDING + BACK_BUTTON_SIZE) {
        Engine::GameEngine::GetInstance().ChangeScene("playground");
        return;
    }
    
    // 處理其他按鈕點擊
    for (size_t i = 1; i < buttons.size(); ++i) {  // 從第二個按鈕開始
        const auto& button = buttons[i];
        if (mouse_x >= button.x && mouse_x <= button.x + button.width &&
            mouse_y >= button.y && mouse_y <= button.y + button.height) {
            if (button.text == "註冊" || button.text == "登入") {
                SubmitForm();
            } else if (button.text == "返回登入" || button.text == "還沒有帳號？點我註冊") {
                // 清空所有輸入框和按鈕
                input_boxes.clear();
                buttons.clear();
                show_error = false;
                
                // 切換模式
                is_register_mode = !is_register_mode;
                
                // 重新初始化 UI
                InitializeUI();
            } else if (button.text == "登出") {
                Logout();
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

// HTTP 回調函數
size_t Login::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t realsize = size * nmemb;
    userp->append((char*)contents, realsize);
    return realsize;
}

// 發送 HTTP 請求
std::string Login::MakeHttpRequest(const std::string& endpoint, const std::string& method, const nlohmann::json& data) {
    CURL* curl = curl_easy_init();
    std::string response;
    
    if (curl) {
        std::string url = std::string(API_BASE_URL) + endpoint;
        std::string data_str = data.dump();
        
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        if (!auth_token.empty()) {
            std::string auth_header = "Authorization: Bearer " + auth_token;
            headers = curl_slist_append(headers, auth_header.c_str());
        }
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        if (!data_str.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_str.c_str());
        }
        
        CURLcode res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    return response;
}

// 修改 LoginUser 函數
bool Login::LoginUser(const std::string& email, const std::string& password) {
    nlohmann::json data = {
        {"email", email},
        {"password", password}
    };
    
    std::string response = MakeHttpRequest("/token", "POST", data);
    
    try {
        auto json_response = nlohmann::json::parse(response);
        if (json_response.contains("access_token")) {
            auth_token = json_response["access_token"];
            
            if (json_response.contains("user")) {
                auto user_data = json_response["user"];
                current_user.username = user_data["username"];
                current_user.email = user_data["email"];
                current_user.created_at = user_data["created_at"];
            }
            show_error = false;
            return true;
        } else {
            if (json_response.contains("detail")) {
                error_message = "登入失敗：" + json_response["detail"].get<std::string>();
            } else {
                error_message = "登入失敗：無效的回應格式";
            }
            show_error = true;
            return false;
        }
    } catch (const nlohmann::json::parse_error& e) {
        error_message = "登入失敗：無法解析伺服器回應";
        show_error = true;
        return false;
    } catch (const std::exception& e) {
        error_message = "登入失敗：" + std::string(e.what());
        show_error = true;
        return false;
    }
}

// 修改 RegisterUser 函數
bool Login::RegisterUser(const std::string& username, const std::string& email, const std::string& password) {
    nlohmann::json data = {
        {"username", username},
        {"email", email},
        {"password", password}
    };
    
    std::string response = MakeHttpRequest("/register", "POST", data);
    
    try {
        auto json_response = nlohmann::json::parse(response);
        if (json_response.contains("access_token")) {
            auth_token = json_response["access_token"];
            
            if (json_response.contains("user")) {
                auto user_data = json_response["user"];
                current_user.username = user_data["username"];
                current_user.email = user_data["email"];
                current_user.created_at = user_data["created_at"];
            }
            show_error = false;
            return true;
        } else {
            if (json_response.contains("detail")) {
                error_message = "註冊失敗：" + json_response["detail"].get<std::string>();
            } else {
                error_message = "註冊失敗：無效的回應格式";
            }
            show_error = true;
            return false;
        }
    } catch (const nlohmann::json::parse_error& e) {
        error_message = "註冊失敗：無法解析伺服器回應";
        show_error = true;
        return false;
    } catch (const std::exception& e) {
        error_message = "註冊失敗：" + std::string(e.what());
        show_error = true;
        return false;
    }
}

// 修改 SubmitForm 函數
void Login::SubmitForm() {
    if (is_register_mode) {
        // 處理註冊
        if (input_boxes.size() >= 3) {
            std::string username = input_boxes[0].text;
            std::string email = input_boxes[1].text;
            std::string password = input_boxes[2].text;
            
            if (RegisterUser(username, email, password)) {
                std::cout << "註冊成功！" << std::endl;
                // 註冊成功後自動登入
                if (LoginUser(email, password)) {
                    std::cout << "自動登入成功！" << std::endl;
                    // 切換到遊戲場景
                    Engine::GameEngine::GetInstance().ChangeScene("playground");
                } else {
                    std::cout << "自動登入失敗！" << std::endl;
                }
            } else {
                std::cout << "註冊失敗！" << std::endl;
            }
        }
    } else {
        // 處理登入
        if (input_boxes.size() >= 2) {
            std::string email = input_boxes[0].text;
            std::string password = input_boxes[1].text;
            
            if (LoginUser(email, password)) {
                std::cout << "登入成功！" << std::endl;
                // 切換到遊戲場景
                Engine::GameEngine::GetInstance().ChangeScene("playground");
            } else {
                std::cout << "登入失敗！" << std::endl;
            }
        }
    }
    std::cout << "Error: " << error_message << std::endl;
}

void Login::Logout() {
    // 清除用戶資料
    current_user.username.clear();
    current_user.email.clear();
    current_user.created_at.clear();
    auth_token.clear();
    
    // 清空所有輸入框和按鈕
    input_boxes.clear();
    buttons.clear();
    show_error = false;
    
    // 重新初始化 UI
    InitializeUI();
}

std::string Login::GetCurrentUserEmail() const {
    return current_user.email;
}

std::string Login::GetCurrentUsername() const {
    return current_user.username;
}

std::string Login::GetCurrentUserCreatedAt() const {
    return current_user.created_at;
}