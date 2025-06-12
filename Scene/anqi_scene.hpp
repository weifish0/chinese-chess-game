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
//int score;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class AnqiScene final : public Engine::IScene {
public:
    Group* ChessGroup;
    Group* UIGroup;
    Engine::Label *UIBlack;
    Engine::Label *UIRed;
    explicit AnqiScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    bool ClickCheck(Engine::Point pos);
    void ChangeRound();
    void InitializeRound(ChessColor color);
    void RecordTemp(Chess* chess,int i,int j);
    bool CannonValidEating(int j,int i,int temp_y,int temp_x);
    void Go(Engine::Point pos,int j,int i);
    void ConstructUI(ChessColor color);
    void ButtonClick(int id);
    void Target(int j,int i,int temp_y,int temp_x);
    bool Edible(Chess* eater,Chess* prey);
    // void PlayOnClick(int stage);
    // void SettingsOnClick(int stage);
};
#endif


