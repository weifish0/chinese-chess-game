#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <memory>
#include "Engine/IObject.hpp"

class Player : public Engine::IObject {
private:
    float x, y;          // 玩家位置
    float speed;         // 移動速度
    float size;          // 玩家大小
    std::shared_ptr<ALLEGRO_BITMAP> sprite_up;
    std::shared_ptr<ALLEGRO_BITMAP> sprite_down;
    std::shared_ptr<ALLEGRO_BITMAP> sprite_left;
    std::shared_ptr<ALLEGRO_BITMAP> sprite_right;
    std::shared_ptr<ALLEGRO_BITMAP> current_sprite;  // 當前顯示的精靈

    // 動畫相關
    int frame_width;     // 單個動畫幀的寬度
    int frame_height;    // 動畫幀的高度
    int current_frame;   // 當前動畫幀
    float animation_timer; // 動畫計時器
    float animation_speed; // 動畫速度（幀/秒）
    bool is_moving;      // 是否正在移動

public:
    Player(float startX = 400, float startY = 300);
    ~Player();

    // 移動控制
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    // IObject 介面實現
    void Draw() const override;
    void Update(float deltaTime) override;

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getSize() const { return size; }
};

#endif // PLAYER_HPP 