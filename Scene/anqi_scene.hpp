#ifndef PlaySCENE_HPP
#define PlaySCENE_HPP


#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Chess/chess.hpp"
extern bool mouseJustClicked;
extern bool lastMouseDown;
extern float pi;
class AnqiRecord{
    public:
        ChessColor color;
        ChessType type;
        int operation;
        ChessColor target_color;
        ChessType target_type;
        int target_num;
        std::pair<int,int> start;
        std::pair<int,int> destination;  
};
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class AnqiScene final : public Engine::IScene {
public:
    ALLEGRO_SAMPLE_ID bgmId;
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
    void Target(int j,int i);
    bool Edible(Chess* eater,Chess* prey);
    void ShowPeace();
    void Recover(AnqiRecord* first,AnqiRecord* second);
    void RecoverValid(int& regret_time);
    std::string GetImage(ChessColor color,ChessType type);
    void CannonTarget(int des_j,int des_i,int& mark);
    void StopSign();
    // void PlayOnClick(int stage);
    // void SettingsOnClick(int stage);
};
#endif


