#ifndef ANQI_STARTSCENE_HPP
#define ANQI_STARTSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class Anqi_StartScene final : public Engine::IScene {
public:
    explicit Anqi_StartScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void SettingsOnClick(int stage);
};
#endif
