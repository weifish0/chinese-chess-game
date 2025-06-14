#ifndef START_SCENE_HPP
#define START_SCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class StartScene final : public Engine::IScene {
public:
    explicit StartScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick();

};
#endif // START_SCENE_HPP
