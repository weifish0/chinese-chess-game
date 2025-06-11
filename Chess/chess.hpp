#ifndef CHESS_HPP
#define CHESS_HPP
#include <allegro5/base.h>
#include <list>
#include <string>
#include <vector>
#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"
#include <deque>

enum ChessType {
    SOLDIER,
    CANNON,
    HORSE,
    CAR,
    ELEPHANT,
    KNIGHT,
    KING
};

enum ChessColor {
    RED,
    BLACK
};
class Chess : public Engine::Sprite {
protected:
    std::string img;
    Sprite frame;
    bool open;
    ChessType type;
    ChessColor color;
    int number;
    

public:
    bool clicking;
    bool justOpen;
    Chess(std::string img, float x, float y,ChessType type,ChessColor color,int number);
    void Open();
    //void Update();
    void Draw() const override;
    bool Check_open();
    void Move(Engine::Point pos);
    ChessColor getColor();
    ChessType getType() const ;
    void Click();
    void ClickCancel();
    int getNumber();
    // static int speed_buff_index;
    // bool Enabled = true;
    // bool Preview = false;
    // Enemy *Target = nullptr;
    // Turret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price, float coolDown);
    // void Update(float deltaTime) override;
    // void Draw() const override;
    // int GetPrice() const;
    // void level_up();
    
};
// extern std::deque<Turret*> tur;
// extern const std::vector<float> speed_buff;
#endif   // TURRET_HPP

