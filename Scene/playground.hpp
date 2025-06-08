#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "Player/player.hpp"

class Playground final : public Engine::IScene {
private:
    Player* player;
    std::shared_ptr<ALLEGRO_BITMAP> background;  // 背景圖片

    // 螢幕邊界
    static constexpr float SCREEN_LEFT = 0;
    static constexpr float SCREEN_RIGHT = 2666;  // 假設螢幕寬度為800
    static constexpr float SCREEN_TOP = 0;
    static constexpr float SCREEN_BOTTOM = 1500;  // 假設螢幕高度為600

public:
    explicit Playground() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;  // 覆寫 Draw 方法來繪製背景
};

#endif // PLAYGROUND_HPP 