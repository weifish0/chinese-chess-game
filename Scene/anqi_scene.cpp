#include <allegro5/allegro_audio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <functional>
#include <memory>
#include <string>
#include <ostream>
#include <iostream>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"

#include "anqi_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Chess/chess.hpp"

#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <chrono>
#include <vector>
#include <cmath>
#include <set>
bool lastMouseDown = false;
bool mouseJustClicked = false;
Chess* temp = nullptr;
int harmony_step = 0;
int red_attack_step = 0;
int black_attack_step = 0;
int temp_x;
int temp_y;
int color = -1;
int black_remain = 16;
int red_remain = 16;
int attack_tick = 0;
int harmony_tick = 0;
int harmony_flag = 0;
int attack_flag = 0;
int attack_mark = 0;
Engine::Label* harmony_warn;
Engine::Label* attack_warn;
std::set<Chess*> red_attack_target;
std::set<Chess*> black_attack_target;
Chess* red_attacker;
Chess* black_attacker;
enum Turn{
    UNKNOWNED,
    RED_TURN,
    BLACK_TURN,
};
Turn turn = UNKNOWNED;
std::vector<std::vector<Chess*>> chessPositions(4, std::vector<Chess*>(8, nullptr));

void AnqiScene::Initialize() {
    
    attack_flag = 0;
    harmony_flag = 0;
    color = -1;
    black_remain = 16;
    red_remain = 16;
    harmony_step = 0;
    red_attack_step = 0;
    black_attack_step = 0;
    red_attack_target.clear();
    black_attack_target.clear();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    //Engine::ImageButton *btn;
    //al_init_image_addon();

    
    
    
    //image->Position = Point(halfW - 400, halfH - 200);
    AddNewObject(new Engine::Image("chess_board.png", 333, 300 , 2000 , 1200 , 0, 0));
    AddNewObject(ChessGroup = new Group());
    AddNewObject(UIGroup = new Group());
    Chess* chess;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::vector<int> order(32);
    for (int i = 0; i < 32; ++i) {
        order[i] = i+1;
    }
    std::shuffle(order.begin(), order.end(), std::default_random_engine(seed));
    harmony_warn = new Engine::Label("空步判和", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(harmony_warn);
    attack_warn = new Engine::Label("長捉禁手", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(attack_warn);

    std::vector<int> black_num(7,0);
    std::vector<int> red_num(7,0);
    for(int j=0;j<4;j++){
        for(int i=0;i<8;i++){
            int temp = order[j*8+i];
            if(temp == 1){
                ChessGroup->AddNewObject(chess = new Chess("black_king.png", 635 + i * 200, 600 + j * 200, KING , BLACK,black_num[0]));
                chessPositions[j][i] = chess;  
                black_num[0]++;
            }
            else if(temp >=2 && temp<=3){
                ChessGroup->AddNewObject(chess = new Chess("black_knight.png", 635 + i * 200, 600 + j * 200 ,KNIGHT, BLACK,black_num[1]));
                chessPositions[j][i] = chess;  
                black_num[1]++;
            }
            else if(temp >=4 && temp <= 5){
                ChessGroup->AddNewObject(chess = new Chess("black_elephant.png", 635 + i * 200, 600 + j * 200 , ELEPHANT, BLACK,black_num[2]));
                chessPositions[j][i] = chess;
                black_num[2]++;
            }
            else if(temp >=6 && temp <= 7){
                ChessGroup->AddNewObject(chess = new Chess("black_car.png", 635 + i * 200, 600 + j * 200, CAR, BLACK,black_num[3]));
                chessPositions[j][i] = chess;
                black_num[3]++;
            }
            else if(temp >=8 && temp <= 9){
                ChessGroup->AddNewObject(chess = new Chess("black_horse.png", 635 + i * 200, 600 + j * 200, HORSE, BLACK,black_num[4]));
                chessPositions[j][i] = chess;
                black_num[4]++;
            }
            else if(temp >=10 && temp <= 11){
                ChessGroup->AddNewObject(chess = new Chess("black_cannon.png", 635 + i * 200, 600 + j * 200, CANNON, BLACK,black_num[5]));
                chessPositions[j][i] = chess;
                black_num[5]++;
            }
            else if(temp >=12 && temp <= 16){
                ChessGroup->AddNewObject(chess = new Chess("black_soldier.png", 635 + i * 200, 600 + j * 200, SOLDIER,BLACK,black_num[6]));
                chessPositions[j][i] = chess;
                black_num[6]++;
            }
            else if(temp == 17){
                ChessGroup->AddNewObject(chess = new Chess("red_king.png", 635 + i * 200, 600 + j * 200, KING, RED,red_num[0]));
                chessPositions[j][i] = chess;
                black_num[0]++;
            }
            else if(temp >= 18 && temp <= 19){
                ChessGroup->AddNewObject(chess = new Chess("red_knight.png", 635 + i * 200, 600 + j * 200, KNIGHT, RED,red_num[1]));
                chessPositions[j][i] = chess;
                black_num[1]++;
            }
            else if(temp >= 20 && temp <= 21){
                ChessGroup->AddNewObject(chess = new Chess("red_elephant.png", 635 + i * 200, 600 + j * 200,ELEPHANT, RED,red_num[2]));
                chessPositions[j][i] = chess;
                black_num[2]++;
            }
            else if(temp >= 22 && temp <= 23){
                ChessGroup->AddNewObject(chess = new Chess("red_car.png", 635 + i * 200, 600 + j * 200, CAR, RED,red_num[3]));
                chessPositions[j][i] = chess;
                black_num[3]++;
            }
            else if(temp >= 24 && temp <= 25){
                ChessGroup->AddNewObject(chess = new Chess("red_horse.png", 635 + i * 200, 600 + j * 200, HORSE, RED,red_num[4]));
                chessPositions[j][i] = chess;
                black_num[4]++;
            }
            else if(temp >= 26 && temp <= 27){
                ChessGroup->AddNewObject(chess = new Chess("red_cannon.png", 635 + i * 200, 600 + j * 200, CANNON, RED,red_num[5]));
                chessPositions[j][i] = chess;
                black_num[5]++;
            }
            else if(temp >= 28 && temp <= 32){
                ChessGroup->AddNewObject(chess = new Chess("red_soldier.png", 635 + i * 200, 600 + j * 200, SOLDIER, RED,red_num[6]));
                chessPositions[j][i] = chess;
                black_num[6]++;
            }
        }
        
    }


}

bool AnqiScene::Edible(Chess* eater,Chess* prey){
    if(eater->getType()==SOLDIER&&prey->getType()==KING){
        return true;
    }
    else if(eater->getType()>=prey->getType()&&!(eater->getType()==KING&&prey->getType()==SOLDIER)){
        return true;
    }
    else{
        return false;
    }
}



    

bool AnqiScene::CannonValidEating(int j,int i,int temp_y,int temp_x){
    //std::cout << j << " " << i << " " << temp_y << " " << temp_x << " " <<std::endl;
    if((j==temp_y)==(i==temp_x)){
        return false;
    }
    int count = 0;
    int min,max;
    if(j==temp_y){
        if(i>temp_x){
            max = i;
            min = temp_x;
        }
        else{
            max = temp_x;
            min = i;
        }
        for(int k=min+1;k<=max-1;k++){
            if(chessPositions[j][k]!=nullptr){
                count++;
            }
                
        }
    }
    else{

    
        if(j>temp_y){
            max = j;
            min = temp_y;
        }
        else{
            max = temp_y;
            min = j;
        }
        for(int k=min+1;k<=max-1;k++){
            if(chessPositions[k][i]!=nullptr){
                count++;
            }
        }
    }
    //std::cout << count <<std::endl;

    if(count==1)
        return true;
    else
        return false;
}
void AnqiScene::ConstructUI(ChessColor color){
    
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    float rx,bx;
    rx = color==RED ? 0: w-300;
    bx = color==BLACK ? 0:w-312.5;
    AddNewObject(new Engine::Image("black_win.png", bx, 0 , 300 , 300 , 0, 0));
    AddNewObject(new Engine::Image("red_win.png", rx, 0 , 312.5 , 300 , 0, 0));
    // Button 1
    // btn = new Engine::ImageButton("start/floor.png", "start/dirt.png", halfW - 200, halfH / 2 - 300, 400, 150);
    // btn->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 1));
    // AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("選項", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 255, 0.5, 0.5));
}

void AnqiScene::InitializeRound(ChessColor color){
    turn = color==RED ? BLACK_TURN : RED_TURN;
    if(turn==RED_TURN)
        std::cout << "Red turn!\n";
    else
        std::cout<< "Black turn!\n";
    ConstructUI(color);
}

void AnqiScene::ChangeRound(){
    turn = turn==RED_TURN ? BLACK_TURN : RED_TURN;
    if(turn==RED_TURN)
        std::cout << "Red turn!\n";
    else
        std::cout<< "Black turn!\n";
    
}

void AnqiScene::RecordTemp(Chess* chess,int i,int j){
    chess->Click();
    temp = chess;
    temp_x = i;
    temp_y = j;
}


void AnqiScene::Go(Engine::Point pos,int j,int i){                         
    temp->Move(pos);
    chessPositions[j][i] = temp;
    chessPositions[temp_y][temp_x] = nullptr;
    temp->ClickCancel();
    temp = nullptr;
    ChangeRound();
}



void AnqiScene::Update(float deltaTime) {
    //std::cout << attack_tick << std::endl;
    attack_tick++;
    harmony_tick++;
    if(harmony_tick>10000000){
        harmony_tick = 0;
    }
    if(attack_tick>10000000){
        attack_tick = 0;
    }
    int cur_step = (turn==RED_TURN)?black_attack_step:red_attack_step;
    if((cur_step>=5&&cur_step<=10||attack_mark)){
        if(!attack_flag){
            attack_tick = 0;
            attack_flag = 1;
        }
        if(attack_tick<45){
            std::cout << "yes\n";
            attack_warn->Text = "長捉禁手" + std::to_string(cur_step) + "/10";
            if(cur_step==10){
                attack_warn->Text = "長捉禁手" + std::to_string(cur_step) + "/10,請移動其他旗子"; 
            }
            else if(attack_mark){
                attack_warn->Text = "長捉禁手10/10,請移動其他旗子!"; 
            }
            attack_warn->Color = al_map_rgba(255,255,255,255);
        }
        else{
            attack_warn->Color = al_map_rgba(255,255,255,0);
            attack_mark = 0;
        }
    }
    else{
        attack_warn->Color = al_map_rgba(255,255,255,0);
    }
    
    if(harmony_step>=25&&harmony_step<50&&!(cur_step>=5&&cur_step<=10)){
        if(!harmony_flag){
            harmony_tick = 0;
            harmony_flag = 1;
        }
        if(harmony_tick<45){
            harmony_warn->Text = "空步判和" + std::to_string(harmony_step) + "/50";
            harmony_warn->Color = al_map_rgba(255 , 255, 255, 255);
        }
        else{
            harmony_warn->Color = al_map_rgba(255,255,255,0);
        }
        
    }
    else if(harmony_step==50){
        Engine::GameEngine::GetInstance().ChangeScene("harmony");
    }
    else{
        harmony_warn->Color = al_map_rgba(255 , 255, 255, 0);
    }
    if(black_remain==0){
        Engine::GameEngine::GetInstance().ChangeScene("red_win");
    }
    else if(red_remain==0){
        Engine::GameEngine::GetInstance().ChangeScene("black_win");
    }
    // Update all chess pieces
    ALLEGRO_MOUSE_STATE mouse;
    al_get_mouse_state(&mouse);
    bool mouseNowDown = al_mouse_button_down(&mouse, 1); // Left mouse button

    mouseJustClicked = (mouseNowDown && !lastMouseDown);  // Rising edge
    lastMouseDown = mouseNowDown;
    
    
    for(int j=0;j<4;j++){
        for(int i=0;i<8;i++){
            Chess* chess = chessPositions[j][i];
            Engine::Point pos;
            
            if(!chess){
                pos.x = 635+i*200;
                pos.y = 600+j*200;
            }
            if(chess && ClickCheck(chess->Position)&& mouseJustClicked){
                if(!chess->Check_open()){
                    harmony_step = 0;
                    int& attack_step = (turn==RED_TURN)?red_attack_step:black_attack_step;
                    attack_step = 0;
                    chess->Open();
                    if(turn==UNKNOWNED)
                        InitializeRound(chess->getColor());
                    else
                        ChangeRound();
                    if(temp){
                        temp->ClickCancel();
                        temp = nullptr;
                    }
                }
                else{
                    if(temp){//have select
                        if(chess->getColor()==color){
                            temp->ClickCancel();
                            RecordTemp(chess,i,j);
                        }
                        else if(temp->getType()==CANNON){//diff color
                            if(CannonValidEating(j,i,temp_y,temp_x)){
                                int& attack_step = (turn==RED_TURN)?red_attack_step:black_attack_step;
                                attack_step = 0;
                                pos = chess->Position;
                                if(chess->getColor()==RED)
                                    red_remain--;
                                else
                                    black_remain--;

                                ChessGroup->RemoveObject(chess->GetObjectIterator());
                                Go(pos,j,i);
                                harmony_step = 0;
                            }
                        }
                        else if(abs(temp_y-j)+abs(temp_x-i)==1){//diff color
                            if(Edible(temp,chess)){
                                int& attack_step = (turn==RED_TURN)?red_attack_step:black_attack_step;
                                attack_step = 0;
                                pos = chess->Position;
                                if(chess->getColor()==RED)
                                    red_remain--;
                                else
                                    black_remain--;
                                ChessGroup->RemoveObject(chess->GetObjectIterator());
                                Go(pos,j,i);
                                harmony_step = 0;
                            }
                               
                            else//can not eat
                                std:: cout<< "invalid!\n";
                        }
                        else
                            std:: cout<< "invalid!\n";
                    }
                    else{//have not select
                        color = chess->getColor();
                        if(color==RED&&turn==RED_TURN||color==BLACK&&turn==BLACK_TURN){
                            RecordTemp(chess,i,j);
                            if(color==0){
                                std::cout << "RED\n";
                            }
                            else
                                std::cout << "BLACK\n";
                        }
                            
                        else
                            std::cout << "Invalid Selection!\n";
                    }
                }
            }
            else if((!chess)&&ClickCheck(pos)&&temp&&mouseJustClicked){
                Chess* cur_attacker = (temp->getColor()==RED)?red_attacker:black_attacker;
                int attack_step = (temp->getColor()==RED)?red_attack_step:black_attack_step;
                std::cout << "attack_step:" << attack_step << std::endl;
                std::cout << "harmony_step:" << harmony_step <<std::endl;
                
                std::cout<< "100\n";
                if(abs(temp_y-j)+abs(temp_x-i)==1){//!(attack_step==10&&temp==attacker)
                    if(!(attack_step==10&&temp==cur_attacker)){
                        Target(j,i,temp_y,temp_x);
                        Go(pos,j,i);//move
                        harmony_step++;
                        harmony_flag = 0;
                    }
                    
                    else{
                        if(attack_step==10){
                            attack_mark = 1;
                            attack_tick = 0;
                        }
                            
                    }
                }
            }
        }  
    }

}

void AnqiScene::Target(int j,int i,int temp_y,int temp_x){
    std::cout << "200\n";
    int color = temp->getColor();
    std::vector<std::pair<int,int>> direction = {
        {-1,0},//up
        {1,0},//down
        {0,-1},//left
        {0,1}///right
    };
    std::set<Chess*>& cur_attack_target = (color==RED)?red_attack_target:black_attack_target;
    int& cur_attack_step = (color==RED)?red_attack_step:black_attack_step;
    Chess*& cur_attacker = (color==RED)?red_attacker:black_attacker;
    if(cur_attack_target.empty()){
        for(auto dir:direction){
            int dj = dir.first;
            int di = dir.second;
            if(j+dj>=0&&j+dj<4&&i+di>=0&&i+di<8){
                if(chessPositions[j+dj][i+di]!=nullptr&&(j+dj!=temp_y||i+di!=temp_x)){
                    cur_attack_target.insert(chessPositions[j+dj][i+di]);
                }
            }
        }
    }
    else{
        int mark = 0;
        std::set<Chess*> update_attack_target;
        for(auto dir:direction){
            int dj = dir.first;
            int di = dir.second;
            if(j+dj>=0&&j+dj<4&&i+di>=0&&i+di<8){
                if(chessPositions[j+dj][i+di]!=nullptr&&(j+dj!=temp_y||i+di!=temp_x)){
                    Chess* chess = chessPositions[j+dj][i+di];
                    if(cur_attack_target.find(chess)!=cur_attack_target.end()){
                        mark = 1;
                    }
                    update_attack_target.insert(chess);
                }
            }
        }
        cur_attack_target = update_attack_target;
        if(mark)
            cur_attack_step++;
        else
            cur_attack_step = 0;

        std::cout << mark << std::endl;
    }
    if(!cur_attack_target.empty()){
        cur_attacker = temp;
        attack_flag = 0;
    }
    else
        cur_attacker = nullptr;

    // if(cur_attacker){
    //     std::cout << cur_attacker->getType() << " " << cur_attacker->getColor() << " " << cur_attacker->getNumber() << std::endl;
    //     std::cout << cur_attack_step << std::endl;
    // }
    std::cout << "attack_flag:" << attack_flag << std::endl;

}

void AnqiScene::ButtonClick(int id){
    //std:: << id <<std::endl;
}




void AnqiScene::Terminate() {
    IScene::Terminate();
}

bool AnqiScene::ClickCheck(Engine::Point pos){
    // Check if the chess piece is clicked
    ALLEGRO_MOUSE_STATE mouse;
    al_get_mouse_state(&mouse);
    if(al_mouse_button_down(&mouse, 1)){
        if (pos.x - 90 <= Engine::GameEngine::GetInstance().GetMousePosition().x &&
        Engine::GameEngine::GetInstance().GetMousePosition().x <= pos.x + 90 &&
        pos.y - 90 <= Engine::GameEngine::GetInstance().GetMousePosition().y &&
        Engine::GameEngine::GetInstance().GetMousePosition().y <= pos.y + 90) {
            return true;
        }
        else{
            return false;
        }
    }
    
    return false;
}
