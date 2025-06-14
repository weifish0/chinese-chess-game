#ifndef REDWINSCENE_HPP
#define REDWINSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class Red_Win_Scene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
public:

    explicit Red_Win_Scene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
};
#endif
