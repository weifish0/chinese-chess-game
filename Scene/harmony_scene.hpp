#ifndef HARMONYSCENE_HPP
#define HARMONYSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class HarmonyScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
public:
    explicit HarmonyScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
};
#endif
