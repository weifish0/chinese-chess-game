#include "player.hpp"
#include "Engine/Resources.hpp"
#include <iostream>

Player::Player(float startX, float startY) {
    x = startX;
    y = startY;
    speed = 3.0f;
    size = 30.0f;

    // 載入 sprites
    sprite_up = Engine::Resources::GetInstance().GetBitmap("Player/PlayerUp.png");
    sprite_down = Engine::Resources::GetInstance().GetBitmap("Player/PlayerDown.png");
    sprite_left = Engine::Resources::GetInstance().GetBitmap("Player/PlayerLeft.png");
    sprite_right = Engine::Resources::GetInstance().GetBitmap("Player/PlayerRight.png");
    
    // 預設顯示向下看的 sprite 
    current_sprite = sprite_down;

    // 初始化動畫相關變數
    if (current_sprite) {
        frame_width = al_get_bitmap_width(current_sprite.get()) / 4;  // 每個 sprite 有4幀
        frame_height = al_get_bitmap_height(current_sprite.get());
    }
    current_frame = 0;
    animation_timer = 0.0f;
    animation_speed = 8.0f;  // 每秒8幀
    is_moving = false;
}

Player::~Player() {
    // 不需要手動釋放 sprite 圖片，因為 shared_ptr 會自動管理
}

void Player::moveUp() {
    y -= speed;
    current_sprite = sprite_up;
    is_moving = true;
}

void Player::moveDown() {
    y += speed;
    current_sprite = sprite_down;
    is_moving = true;
}

void Player::moveLeft() {
    x -= speed;
    current_sprite = sprite_left;
    is_moving = true;
}

void Player::moveRight() {
    x += speed;
    current_sprite = sprite_right;
    is_moving = true;
}

void Player::Draw(float draw_x, float draw_y, float scale_x, float scale_y) {
    if (current_sprite) {
        float sprite_width = frame_width;
        float sprite_height = frame_height;
        al_draw_scaled_bitmap(
            current_sprite.get(),
            current_frame * frame_width, 0,
            frame_width, frame_height,
            draw_x,
            draw_y,
            sprite_width * scale_x,
            sprite_height * scale_y,
            0
        );
    }
}

void Player::Update(float deltaTime) {
    // std::cout << "player(" << x << ", " << y << ")" << std::endl;
    if (is_moving) {
        // 更新動畫計時器
        animation_timer += deltaTime;
        
        // 當計時器達到一幀的時間時，更新當前幀
        if (animation_timer >= 1.0f / animation_speed) {
            animation_timer = 0.0f;
            current_frame = (current_frame + 1) % 4;  // 循環播放4幀
        }
    } else {
        // 如果沒有移動，重置到第一幀
        current_frame = 0;
        animation_timer = 0.0f;
    }
    
    // 重置移動狀態
    is_moving = false;
} 