#ifndef PlaySCENE_HPP
#define PlaySCENE_HPP


#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Chess/chess.hpp"
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class PlayScene final : public Engine::IScene {
public:
    Group* ChessGroup;
    Group* UIGroup;
    Engine::Label *UIBlack;
    Engine::Label *UIRed;
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    bool ClickCheck(Engine::Point pos);
    void ChangeRound();
    void InitializeRound(ChessColor color);
    void RecordTemp(Chess* chess,int i,int j);
    bool CannonValidEating(int j,int i,int temp_y,int temp_x);
    void Move(Engine::Point pos,int j,int i);
    void ConstructUI();
    // void PlayOnClick(int stage);
    // void SettingsOnClick(int stage);
};
#endif


