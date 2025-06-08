#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Engine/IScene.hpp"
#include "Player/player.hpp"

class Playground final : public Engine::IScene {
private:
    Player* player;

public:
    explicit Playground() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
};

#endif // PLAYGROUND_HPP 