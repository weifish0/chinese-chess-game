#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <utility>
#include <iostream>

#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "chess.hpp"
#include "UI/Component/Image.hpp"
#include "Engine/Resources.hpp"
#include "Scene/setting_scene.hpp"

std::string frame_img = "frame.png";
Chess::Chess(std::string img, float x, float y,ChessType type,ChessColor color,int number): Sprite(close_img, x, y,180,180),frame(frame_img,x,y,180,180){
    this->img = img;
    this->open = false;
    this->type = type;
    this->color = color;
    this->clicking = false;
    this->number = number;
}

// void Chess::Update() {
    
// }
void Chess::Draw() const {
    // if (Preview) {
    //     al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgba(0, 255, 0, 50));
    // }
    //img.Draw();
    Sprite::Draw();
    if(clicking){
        frame.Draw();
    }
    // if (PlayScene::DebugMode) {
    //     // Draw target radius.
    //     al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(0, 0, 255), 2);
    // }
}
ChessColor Chess::getColor(){
    return color;
}
ChessType Chess::getType() const {
    return type;
}

void Chess::Open(){
    this->open = true;
    bmp = Engine::Resources::GetInstance().GetBitmap(img);
    //Sprite::Draw();
}

bool Chess::Check_open(){
    return open;
}

void Chess::Move(Engine::Point pos){
    Position.x = pos.x;
    Position.y = pos.y;
    frame.Position.x = pos.x;
    frame.Position.y = pos.y;
    //Draw();
}

void Chess::Click(){
    clicking = true;
}

void Chess::ClickCancel(){
    clicking = false;
}

int Chess::getNumber(){
    return number;
}


