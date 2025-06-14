#ifndef SETTINGSCENE_HPP
#define SETTINGSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
extern int decide;
extern std::string close_img;
class AnqiSettingScene final : public Engine::IScene {
public:
    ALLEGRO_SAMPLE_ID bgmId;
    explicit AnqiSettingScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void SettingsOnClick(int id);
    void BackOnClick(int stage);
};
#endif // STARTSCENE_HPP
