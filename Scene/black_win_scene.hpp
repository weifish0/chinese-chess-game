#ifndef BLACKWINSCENE_HPP
#define BLACKWINSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class Black_Win_Scene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
public:
    explicit Black_Win_Scene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
};
#endif
