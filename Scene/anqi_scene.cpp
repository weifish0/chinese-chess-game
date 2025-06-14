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
#include "setting_scene.hpp"

#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <chrono>
#include <vector>
#include <cmath>
#include <set>
#include <deque>


class Manage{
    public:
        int attack_step;
        int remain;
        int request_time;
        int regret_time;
        int prolong_time;
        Chess* attacker;
        std::set<Chess*> attack_target;

};
class Tick{
    public:
        int attack_tick;
        int turn_tick;
        int prolong_tick;
        int harmony_tick;
        int pointer_tick;
        int peace_tick;
        int regret_tick;
        int regret_invalid_tick;
};
class Flag{
    public:
        int harmony_flag;
        int attack_flag;
        int attack_mark;
        int prolong_mark;
        int peace_flag;
        int peace_mark;
        int regret_flag;
        int regret_mark;
        int regret_invalid_flag;
};
class Sign{
    public:
        Engine::Label* harmony_warn;
        Engine::Label* attack_warn;
        Engine::Label* prolong_warn;
        Engine::Label* peace_warn;
        Engine::Label* peace_ask;
        Engine::Label* regret_warn;
        Engine::Label* timer;
        Engine::Label* request_peace;
        Engine::Label* prolong;
        Engine::Label* regret;
        Engine::Label* regret_invalid;
};
bool lastMouseDown = false;
bool mouseJustClicked = false;
int harmony_step = 0;
int temp_x;
int temp_y;
int color = -1;
float pi = ALLEGRO_PI;
float rx,bx,p1_x,p2_x,t1_x,t2_x;
Chess* temp = nullptr;
Engine::Image* pointer;
Engine::Image* peace_window;
enum Turn{
    UNKNOWNED,
    RED_TURN,
    BLACK_TURN,
};
std::deque<Record*> record;
Turn turn = UNKNOWNED;
Turn first_turn;
std::vector<std::vector<Chess*>> chessPositions(4, std::vector<Chess*>(8, nullptr));

Manage* red;
Manage* black;
Tick* tick;
Flag* flag;
Sign* sign;
void AnqiScene::Initialize() {
    red = new Manage();
    black = new Manage();
    tick = new Tick();
    flag = new Flag();
    sign = new Sign();
    red->prolong_time = 0;
    black->prolong_time = 0;
    red->request_time = 0;
    black->regret_time = 0;
    red->regret_time = 0;
    black->regret_time = 0;
    //tick
    tick->regret_invalid_tick = 0;
    tick->regret_tick = 0;
    tick->turn_tick = 1259;
    tick->pointer_tick = -30;
    tick->prolong_tick = 0;
    tick->peace_tick = 0;
    tick->attack_tick = 0;
    tick->harmony_tick = 0;
    //flag
    flag->regret_invalid_flag = 0;
    flag->regret_flag = 0;
    flag->attack_mark = 0;
    flag->attack_flag = 0;
    flag->harmony_flag = 0;
    flag->prolong_mark = 0;
    flag->attack_mark = 0;
    flag->peace_flag = 0;
    color = -1;
    black->remain = 16;
    red->remain = 16;
    harmony_step = 0;
    red->attack_step = 0;
    black->attack_step = 0;
    red->attack_target.clear();
    black->attack_target.clear();
    record.clear();
    turn = UNKNOWNED;
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
    sign->harmony_warn = new Engine::Label("空步判和", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(sign->harmony_warn);
    sign->attack_warn = new Engine::Label("長捉禁手", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(sign->attack_warn);
    sign->prolong_warn = new Engine::Label("延長次數耗盡!", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(sign->prolong_warn);
    sign->peace_warn = new Engine::Label("求和次數耗盡!", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(sign->peace_warn);
    sign->regret_invalid= new Engine::Label("無法悔棋!", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(sign->regret_invalid);
    sign->regret_warn = new Engine::Label("悔棋次數耗盡!", "font2.ttc", 100, halfW, 140, 255 , 255 ,255 , 0, 0.5, 0.5);
    AddNewObject(sign->regret_warn);

    
    //sign->peace_ask->Color = al_map_rgba(255,255,255,255);
    std::string s;
    std::vector<int> black_num(7,0);
    std::vector<int> red_num(7,0);
    for(int j=0;j<4;j++){
        for(int i=0;i<8;i++){
            int temp = order[j*8+i];
            if(temp == 1){
                s = GetImage(BLACK,KING);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, KING , BLACK,black_num[0]));
                chessPositions[j][i] = chess;  
                black_num[0]++;
            }
            else if(temp >=2 && temp<=3){
                s = GetImage(BLACK,KNIGHT);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200 ,KNIGHT, BLACK,black_num[1]));
                chessPositions[j][i] = chess;  
                black_num[1]++;
            }
            else if(temp >=4 && temp <= 5){
                s = GetImage(BLACK,ELEPHANT);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200 , ELEPHANT, BLACK,black_num[2]));
                chessPositions[j][i] = chess;
                black_num[2]++;
            }
            else if(temp >=6 && temp <= 7){
                s = GetImage(BLACK,CAR);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, CAR, BLACK,black_num[3]));
                chessPositions[j][i] = chess;
                black_num[3]++;
            }
            else if(temp >=8 && temp <= 9){
                s = GetImage(BLACK,HORSE);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, HORSE, BLACK,black_num[4]));
                chessPositions[j][i] = chess;
                black_num[4]++;
            }
            else if(temp >=10 && temp <= 11){
                s = GetImage(BLACK,CANNON);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, CANNON, BLACK,black_num[5]));
                chessPositions[j][i] = chess;
                black_num[5]++;
            }
            else if(temp >=12 && temp <= 16){
                s = GetImage(BLACK,SOLDIER);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, SOLDIER,BLACK,black_num[6]));
                chessPositions[j][i] = chess;
                black_num[6]++;
            }
            else if(temp == 17){
                s = GetImage(RED,KING);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, KING, RED,red_num[0]));
                chessPositions[j][i] = chess;
                black_num[0]++;
            }
            else if(temp >= 18 && temp <= 19){
                s = GetImage(RED,KNIGHT);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, KNIGHT, RED,red_num[1]));
                chessPositions[j][i] = chess;
                black_num[1]++;
            }
            else if(temp >= 20 && temp <= 21){
                s = GetImage(RED,ELEPHANT);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200,ELEPHANT, RED,red_num[2]));
                chessPositions[j][i] = chess;
                black_num[2]++;
            }
            else if(temp >= 22 && temp <= 23){
                s = GetImage(RED,CAR);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, CAR, RED,red_num[3]));
                chessPositions[j][i] = chess;
                black_num[3]++;
            }
            else if(temp >= 24 && temp <= 25){
                s = GetImage(RED,HORSE);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, HORSE, RED,red_num[4]));
                chessPositions[j][i] = chess;
                black_num[4]++;
            }
            else if(temp >= 26 && temp <= 27){
                s = GetImage(RED,CANNON);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, CANNON, RED,red_num[5]));
                chessPositions[j][i] = chess;
                black_num[5]++;
            }
            else if(temp >= 28 && temp <= 32){
                s = GetImage(RED,SOLDIER);
                ChessGroup->AddNewObject(chess = new Chess(s, 635 + i * 200, 600 + j * 200, SOLDIER, RED,red_num[6]));
                chessPositions[j][i] = chess;
                black_num[6]++;
            }
        }
        
    }

    bgmId = AudioHelper::PlayBGM("play_music.ogg");
}

void RecordStep(ChessColor color,ChessType type,int operation,ChessColor target_color,ChessType target_type,std::pair<int,int> start,std::pair<int,int> destination){
    Record* a = new Record();
    a->color = color;
    a->type = type;
    a->operation = operation;
    a->start = start;
    a->destination = destination;
    a->target_color = target_color;
    a->target_type = target_type;
    record.push_back(a);
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
    
    rx = color==RED ? 0: w-312.5;
    bx = color==BLACK ? 0:w-290;
    p2_x = color==BLACK ? w-312.5/2-100:w-145-100;
    p1_x = color==BLACK ? 45:312.5/2-100;
    t2_x = color==BLACK ? w-312.5-200:w-290-200;
    t1_x = color==BLACK ? 490:512.5;
    AddNewObject(new Engine::Image("black_win.png", bx, 0 , 290 , 290 , 0, 0));
    AddNewObject(new Engine::Image("red_win.png", rx, 0 , 312.5 , 300 , 0, 0));
    pointer = new Engine::Image("pointer.png", p2_x, 325 , 200 , 200 , 0, 0);
    AddNewObject(pointer);
    sign->timer = new Engine::Label("20", "font2.ttc", 150, t2_x, 150, 223 , 21 ,22 , 255, 0.5, 0.5);
    AddNewObject(sign->timer);
    
    // Button 1
    btn = new Engine::ImageButton("block2.png", "start/dirt.png", 0, halfH, 300, 150);
    btn->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("投降", "font2.ttc", 70, 150, halfH+75, 255 , 255 ,255 , 255, 0.5, 0.5));

    btn = new Engine::ImageButton("block2.png", "start/dirt.png", 0, halfH+300, 300, 150);
    btn->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 2));
    AddNewControlObject(btn);
    sign->request_peace = new Engine::Label("求和0/2", "font2.ttc", 70, 150, halfH+375, 255 , 255 ,255 , 255, 0.5, 0.5);
    AddNewObject(sign->request_peace);

    btn = new Engine::ImageButton("block2.png", "start/dirt.png", w-300, halfH, 300, 150);
    btn->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 3));
    AddNewControlObject(btn);
    sign->regret = new Engine::Label("悔棋0/3", "font2.ttc", 70, w-150, halfH+75, 255 , 255 ,255 , 255, 0.5, 0.5);
    AddNewObject(sign->regret);

    btn = new Engine::ImageButton("block2.png", "start/dirt.png", w-300, halfH+300, 300, 150);
    btn->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 4));
    AddNewControlObject(btn);
    sign->prolong = new Engine::Label("延長0/3", "font2.ttc", 70, w-150, halfH+375, 255 , 255 ,255 , 255, 0.5, 0.5);
    AddNewObject(sign->prolong);
}

void AnqiScene::InitializeRound(ChessColor color){
    turn = color==RED ? BLACK_TURN : RED_TURN;
    first_turn = (turn==BLACK_TURN)?RED_TURN:BLACK_TURN;
    if(turn==RED_TURN)
        std::cout << "Red turn!\n";
    else
        std::cout<< "Black turn!\n";
    ConstructUI(color);
}

void AnqiScene::ChangeRound(){
    tick->turn_tick = 1259;
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

void AnqiScene::StopSign(){
    sign->harmony_warn->Color = al_map_rgba(255,255,255,0);
    sign->attack_warn->Color = al_map_rgba(255,255,255,0);
    sign->peace_warn->Color = al_map_rgba(255,255,255,0);
    sign->prolong_warn->Color = al_map_rgba(255,255,255,0);
    sign->regret_invalid->Color = al_map_rgba(255,255,255,0);
    sign->regret_warn->Color = al_map_rgba(255,255,255,0);
}

void AnqiScene::Update(float deltaTime) {
    
    

    if(tick->turn_tick==30){
        if(turn==RED_TURN){
            Engine::GameEngine::GetInstance().ChangeScene("black_win");
        }
        else
            Engine::GameEngine::GetInstance().ChangeScene("red_win");
    }

    if(turn!=UNKNOWNED){
        if(!flag->peace_mark)
            tick->turn_tick--;
        int prolong_time = turn==RED_TURN?red->prolong_time:black->prolong_time;
        int peace_time = turn==RED_TURN?red->request_time:black->request_time;
        int regret_time = turn==RED_TURN?red->regret_time:black->regret_time;
        sign->request_peace->Text = "求和" + std::to_string(peace_time) + "/2";
        sign->prolong->Text = "延長" + std::to_string(prolong_time) + "/3";
        sign->regret->Text = "悔棋" + std::to_string(regret_time) + "/3";
        pointer->Position.y = 325 + 10*sin(2*pi*tick->pointer_tick/30);
        sign->timer->Text = std::to_string(tick->turn_tick/60);
        if(turn==first_turn){
            pointer->Position.x = p1_x;
            sign->timer->Position.x = t1_x;
        }
        else{
            pointer->Position.x = p2_x;
            sign->timer->Position.x = t2_x;
        }
    }
    
    //std::cout << attack_tick << std::endl;
    if(tick->pointer_tick>30){
        tick->pointer_tick = -30;
    }
    tick->peace_tick++;
    tick->pointer_tick++;
    tick->attack_tick++;
    tick->harmony_tick++;
    tick->prolong_tick++;
    tick->regret_invalid_tick++;
    tick->regret_tick++;
    if(tick->regret_tick<30&&flag->regret_flag){
        StopSign();
        sign->regret_warn->Color = al_map_rgba(255,255,255,255);
    }
    else{
        sign->regret_warn->Color = al_map_rgba(255,255,255,0);
        flag->regret_flag = 0;
    }

    if(tick->regret_invalid_tick<30&&flag->regret_invalid_flag){
        StopSign();
        sign->regret_invalid->Color = al_map_rgba(255,255,255,255);
    }
    else{
        sign->regret_invalid->Color = al_map_rgba(255,255,255,0);
        flag->regret_invalid_flag = 0;
    }
    if(tick->peace_tick<30&&flag->peace_flag){
        StopSign();
        sign->peace_warn->Color = al_map_rgba(255,255,255,255);
    }
    else{
        sign->peace_warn->Color = al_map_rgba(255,255,255,0);
        flag->peace_flag = 0;
    }
    if(tick->prolong_tick<30&&flag->prolong_mark){
        StopSign();
        sign->prolong_warn->Color = al_map_rgba(255,255,255,255);
    }
    else{
        sign->prolong_warn->Color = al_map_rgba(255,255,255,0);
        flag->prolong_mark = 0;
    }
    if(tick->harmony_tick>10000000){
        tick->harmony_tick = 0;
    }
    if(tick->attack_tick>10000000){
        tick->attack_tick = 0;
    }
    if(tick->peace_tick>10000000){
        tick->peace_tick = 0;
    }
    if(tick->regret_invalid_tick>10000000){
        tick->regret_invalid_tick = 0;
    }
     if(tick->regret_tick>10000000){
        tick->regret_tick = 0;
    }
    if(tick->prolong_tick>10000000){
        tick->prolong_tick = 0;
    }
    int cur_step = (turn==RED_TURN)?black->attack_step:red->attack_step;
    if((cur_step>=5&&cur_step<=10||flag->attack_mark)){
        if(!flag->attack_flag){
            tick->attack_tick = 0;
            flag->attack_flag = 1;
        }
        if(tick->attack_tick<45){
            std::cout << "yes\n";
            sign->attack_warn->Text = "長捉禁手" + std::to_string(cur_step) + "/10";
            if(cur_step==10||flag->attack_mark){
                sign->attack_warn->Text = "長捉禁手10/10,請移動其他旗子!"; 
            }
            StopSign();
            sign->attack_warn->Color = al_map_rgba(255,255,255,255);
        }
        else{
            sign->attack_warn->Color = al_map_rgba(255,255,255,0);
            flag->attack_mark = 0;
        }
    }
    else{
        sign->attack_warn->Color = al_map_rgba(255,255,255,0);
    }
    
    if(harmony_step>=25&&harmony_step<50&&!(cur_step>=5&&cur_step<=10)){
        if(!flag->harmony_flag){
            tick->harmony_tick = 0;
            flag->harmony_flag = 1;
        }
        if(tick->harmony_tick<45){
            sign->harmony_warn->Text = "空步判和" + std::to_string(harmony_step) + "/50";
            StopSign();
            sign->harmony_warn->Color = al_map_rgba(255 , 255, 255, 255);
        }
        else{
            sign->harmony_warn->Color = al_map_rgba(255,255,255,0);
        }
        
    }
    else if(harmony_step==50){
        Engine::GameEngine::GetInstance().ChangeScene("harmony");
    }
    else{
        sign->harmony_warn->Color = al_map_rgba(255 , 255, 255, 0);
    }
    if(black->remain==0){
        Engine::GameEngine::GetInstance().ChangeScene("red_win");
    }
    else if(red->remain==0){
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
            if(chess && ClickCheck(chess->Position)&& mouseJustClicked&&!flag->peace_mark){
                if(!chess->Check_open()){
                    harmony_step = 0;
                    int& attack_step = (turn==RED_TURN)?red->attack_step:black->attack_step;
                    attack_step = 0;
                    chess->Open();
                    RecordStep(chess->getColor(),chess->getType(),-1,RED,KING,{-1,-1},{j,i});
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
                                int& attack_step = (turn==RED_TURN)?red->attack_step:black->attack_step;
                                attack_step = 0;
                                pos = chess->Position;
                                RecordStep(temp->getColor(),temp->getType(),1,chess->getColor(),chess->getType(),{temp_y,temp_x},{j,i});
                                if(chess->getColor()==RED)
                                    red->remain--;
                                else
                                    black->remain--;

                                ChessGroup->RemoveObject(chess->GetObjectIterator());
                                Go(pos,j,i);
                                harmony_step = 0;
                            }
                        }
                        else if(abs(temp_y-j)+abs(temp_x-i)==1){//diff color
                            if(Edible(temp,chess)){
                                int& attack_step = (turn==RED_TURN)?red->attack_step:black->attack_step;
                                attack_step = 0;
                                pos = chess->Position;
                                RecordStep(temp->getColor(),temp->getType(),1,chess->getColor(),chess->getType(),{temp_y,temp_x},{j,i});
                                if(chess->getColor()==RED)
                                    red->remain--;
                                else
                                    black->remain--;
                                ChessGroup->RemoveObject(chess->GetObjectIterator());
                                Go(pos,j,i);
                                harmony_step = 0;
                            }
                               
                            
                        }
                        
                    }
                    else{//have not select
                        color = chess->getColor();
                        if(color==RED&&turn==RED_TURN||color==BLACK&&turn==BLACK_TURN){
                            RecordTemp(chess,i,j);
                        }
                    }
                }
            }
            else if((!chess)&&ClickCheck(pos)&&temp&&mouseJustClicked&&!flag->peace_mark){
                Chess* cur_attacker = (temp->getColor()==RED)?red->attacker:black->attacker;
                int attack_step = (temp->getColor()==RED)?red->attack_step:black->attack_step;
                if(abs(temp_y-j)+abs(temp_x-i)==1){//!(attack_step==10&&temp==attacker)
                    if(!(attack_step==10&&temp==cur_attacker)){
                        RecordStep(temp->getColor(),temp->getType(),0,RED,KING,{temp_y,temp_x},{j,i});
                        Target(j,i,temp_y,temp_x);
                        Go(pos,j,i);//move
                        harmony_step++;
                        flag->harmony_flag = 0;
                    }
                    
                    else{
                        if(attack_step==10){
                            flag->attack_mark = 1;
                            tick->attack_tick = 0;
                        }
                            
                    }
                }
            }
        }  
    }

}

void AnqiScene::Target(int j,int i,int temp_y,int temp_x){
    int color = temp->getColor();
    std::vector<std::pair<int,int>> direction = {
        {-1,0},//up
        {1,0},//down
        {0,-1},//left
        {0,1}///right
    };
    std::set<Chess*>& cur_attack_target = (color==RED)?red->attack_target:black->attack_target;
    int& cur_attack_step = (color==RED)?red->attack_step:black->attack_step;
    Chess*& cur_attacker = (color==RED)?red->attacker:black->attacker;
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
        flag->attack_flag = 0;
    }
    else
        cur_attacker = nullptr;

    // if(cur_attacker){
    //     std::cout << cur_attacker->getType() << " " << cur_attacker->getColor() << " " << cur_attacker->getNumber() << std::endl;
    //     std::cout << cur_attack_step << std::endl;
    // }

}


void AnqiScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
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
Engine::ImageButton* temp1;
Engine::ImageButton* temp2;
Engine::Label* temp3;
Engine::Label* temp4;

void AnqiScene::ShowPeace(){
    flag->peace_mark = 1;
    peace_window = new Engine::Image("block6.png",933,350,800,800,0,0);
    AddNewObject(peace_window);
    sign->peace_ask = new Engine::Label("是否接受求和?", "font2.ttc", 100, 1333, 650, 255 , 255 ,255 , 255, 0.5, 0.5);
    AddNewObject(sign->peace_ask);
    
    temp1 = new Engine::ImageButton("block2.png", "start/dirt.png", 983, 900, 300, 150);
    temp1->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 5));
    AddNewControlObject(temp1);
    temp3 = new Engine::Label("是", "font2.ttc", 70, 983+150, 975, 255 , 255 ,255 , 255, 0.5, 0.5);
    AddNewObject(temp3);

    temp2 = new Engine::ImageButton("block2.png", "start/dirt.png", 1733-350, 900, 300, 150);
    temp2->SetOnClickCallback(std::bind(&AnqiScene::ButtonClick, this, 6));
    AddNewControlObject(temp2);
    temp4 = new Engine::Label("否", "font2.ttc", 70, 1733-350+150, 975, 255 , 255 ,255 , 255, 0.5, 0.5);
    AddNewObject(temp4);

    

}
void AnqiScene::Recover(Record* first,Record* second){
    Chess* chess = chessPositions[first->destination.first][first->destination.second];
    Engine::Point recover_pos;
    std::cout << first->destination.first << " " << first->destination.second << std::endl;
    recover_pos.y = 600+200*first->start.first;//recover
    recover_pos.x = 635+200*first->start.second;//recover
    chess->Move(recover_pos);
    chessPositions[first->start.first][first->start.second] = chess;
    chessPositions[first->destination.first][first->destination.second] = nullptr;
    if(first->operation==1){//if eat
        int i = first->destination.second;
        int j = first->destination.first;
        std::string image = GetImage(first->target_color,first->target_type);//get image
        std::cout << image << std::endl;
        ChessGroup->AddNewObject(chess = new Chess(image, 635 + i * 200, 600 + j * 200 ,first->target_type, first->target_color,1));//recover eaten chess
        chess->Open();
        chessPositions[first->destination.first][first->destination.second] = chess;
    }
    chess = chessPositions[second->destination.first][second->destination.second];
    recover_pos.y = 600+200*second->start.first;
    recover_pos.x = 635+200*second->start.second;
    chess->Move(recover_pos);
    chessPositions[second->start.first][second->start.second] = chess;
    chessPositions[second->destination.first][second->destination.second] = nullptr;
    if(second->operation==1){
        int i = second->destination.second;
        int j = second->destination.first;
        std::string image = GetImage(second->target_color,second->target_type);
        std::cout << image << std::endl;
        ChessGroup->AddNewObject(chess = new Chess(image, 635 + i * 200, 600 + j * 200 ,second->target_type, second->target_color,1));
        chess->Open();
        chessPositions[second->destination.first][second->destination.second] = chess;
    }
    
}
std::string AnqiScene::GetImage(ChessColor color,ChessType type){
    std::string s = "";
    if(color==RED)
        s += "red_";
    else 
        s += "black_";

    if(type==KING){
        s += "king";
    }
    else if(type==KNIGHT){
        s += "knight";
    }
    else if(type==ELEPHANT){
        s += "elephant";
    }
    else if(type==CAR){
        s += "car";
    }
    else if(type==HORSE){
        s += "horse";
    }
    else if(type==CANNON){
        s += "cannon";
    }
    else{
        s += "soldier";
    }
    if(decide){
        s += "3";
    }
    s += ".png";
    return s;

}
void AnqiScene::RecoverValid(int& regret_time){
    if(record.size()<2){
        flag->regret_invalid_flag = 1;
        tick->regret_invalid_tick = 0;
        return;
    }
    Record* a = record.back();
    record.pop_back();
    Record* b = record.back();
    if(a->operation==-1||b->operation==-1){
        flag->regret_invalid_flag = 1;
        tick->regret_invalid_tick = 0;
        record.push_back(a);
        return;
    }
    regret_time++;
    record.pop_back();
    Recover(a,b);
    
}




void AnqiScene::ButtonClick(int id){
    if(id==1&&!flag->peace_mark){
        if(turn == RED_TURN)
            Engine::GameEngine::GetInstance().ChangeScene("black_win");
        else
            Engine::GameEngine::GetInstance().ChangeScene("red_win");
    }
    else if(id==2&&!flag->peace_mark){
        int& request_time = turn==RED_TURN?red->request_time:black->request_time;

        if(request_time<2){
            request_time++;
            ShowPeace();

        }
        else{
            flag->peace_flag = 1;
            tick->peace_tick = 0;
        }
    }
    else if(id==3&&!flag->peace_mark){
        int& regret_time = turn==RED_TURN?red->regret_time:black->regret_time;
        if(regret_time<3){
            RecoverValid(regret_time);
        }
        else{
            flag->regret_flag = 1;
            tick->regret_tick = 0;
        }
    }
    else if(id==4&&!flag->peace_mark){
        int &prolong_time = turn==RED_TURN?red->prolong_time:black->prolong_time;
        
        if(prolong_time<3){
            prolong_time++;
            tick->turn_tick+=600;
            
        }
        else{
            //sign->prolong_warn->Color = al_map_rgba(255,255,255,255);
            tick->prolong_tick = 0;
            flag->prolong_mark = 1;
        }
    }
    else if(id==5){
        Engine::GameEngine::GetInstance().ChangeScene("harmony");
    }
    else if(id==6){
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);
        bool mouseNowDown = al_mouse_button_down(&mouse, 1); // Left mouse button

        mouseJustClicked = (mouseNowDown && !lastMouseDown);  // Rising edge
        lastMouseDown = mouseNowDown;
        flag->peace_mark = 0;
        RemoveObject(temp3->GetObjectIterator());
        RemoveObject(temp4->GetObjectIterator());
        RemoveObject(peace_window->GetObjectIterator());
        RemoveObject(sign->peace_ask->GetObjectIterator());
        RemoveControlObject(temp1->controlIterator,temp1->GetObjectIterator());
        RemoveControlObject(temp2->controlIterator,temp2->GetObjectIterator());
    }
    else{
        return;
    }
}