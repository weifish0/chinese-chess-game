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

#include "play_scene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Chess/chess.hpp"

#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <chrono>
#include <vector>
#include <cmath>
bool lastMouseDown = false;
bool mouseJustClicked = false;
Chess* temp = nullptr;
int temp_x;
int temp_y;
int color = -1;
enum Turn{
    UNKNOWNED,
    RED_TURN,
    BLACK_TURN,
};
Turn turn = UNKNOWNED;
std::vector<std::vector<Chess*>> chessPositions(4, std::vector<Chess*>(8, nullptr));

void PlayScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    //Engine::ImageButton *btn;
    //al_init_image_addon();

    
    
    
    //image->Position = Point(halfW - 400, halfH - 200);
    AddNewObject(new Engine::Image("chess_board.png", 333, 150 , 2000 , 1200 , 0, 0));
    AddNewObject(ChessGroup = new Group());
    Chess* chess;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::vector<int> order(32);
    for (int i = 0; i < 32; ++i) {
        order[i] = i+1;
    }
    std::shuffle(order.begin(), order.end(), std::default_random_engine(seed));

    


    for(int j=0;j<4;j++){
        for(int i=0;i<8;i++){
            int temp = order[j*8+i];
            if(temp == 1){
                ChessGroup->AddNewObject(chess = new Chess("black_king.png", 635 + i * 200, 450 + j * 200, KING , BLACK));
                chessPositions[j][i] = chess;  
            }
            else if(temp >=2 && temp<=3){
                ChessGroup->AddNewObject(chess = new Chess("black_knight.png", 635 + i * 200, 450 + j * 200 ,KNIGHT, BLACK));
                chessPositions[j][i] = chess;  
            }
            else if(temp >=4 && temp <= 5){
                ChessGroup->AddNewObject(chess = new Chess("black_elephant.png", 635 + i * 200, 450 + j * 200 , ELEPHANT, BLACK));
                chessPositions[j][i] = chess;
            }
            else if(temp >=6 && temp <= 7){
                ChessGroup->AddNewObject(chess = new Chess("black_car.png", 635 + i * 200, 450 + j * 200, CAR, BLACK));
                chessPositions[j][i] = chess;
            }
            else if(temp >=8 && temp <= 9){
                ChessGroup->AddNewObject(chess = new Chess("black_horse.png", 635 + i * 200, 450 + j * 200, HORSE, BLACK));
                chessPositions[j][i] = chess;
            }
            else if(temp >=10 && temp <= 11){
                ChessGroup->AddNewObject(chess = new Chess("black_cannon.png", 635 + i * 200, 450 + j * 200, CANNON, BLACK));
                chessPositions[j][i] = chess;
            }
            else if(temp >=12 && temp <= 16){
                ChessGroup->AddNewObject(chess = new Chess("black_soldier.png", 635 + i * 200, 450 + j * 200, SOLDIER,BLACK));
                chessPositions[j][i] = chess;
            }
            else if(temp == 17){
                ChessGroup->AddNewObject(chess = new Chess("red_king.png", 635 + i * 200, 450 + j * 200, KING, RED));
                chessPositions[j][i] = chess;
            }
            else if(temp >= 18 && temp <= 19){
                ChessGroup->AddNewObject(chess = new Chess("red_knight.png", 635 + i * 200, 450 + j * 200, KNIGHT, RED));
                chessPositions[j][i] = chess;
            }
            else if(temp >= 20 && temp <= 21){
                ChessGroup->AddNewObject(chess = new Chess("red_elephant.png", 635 + i * 200, 450 + j * 200,ELEPHANT, RED));
                chessPositions[j][i] = chess;
            }
            else if(temp >= 22 && temp <= 23){
                ChessGroup->AddNewObject(chess = new Chess("red_car.png", 635 + i * 200, 450 + j * 200, CAR, RED));
                chessPositions[j][i] = chess;
            }
            else if(temp >= 24 && temp <= 25){
                ChessGroup->AddNewObject(chess = new Chess("red_horse.png", 635 + i * 200, 450 + j * 200, HORSE, RED));
                chessPositions[j][i] = chess;
            }
            else if(temp >= 26 && temp <= 27){
                ChessGroup->AddNewObject(chess = new Chess("red_cannon.png", 635 + i * 200, 450 + j * 200, CANNON, RED));
                chessPositions[j][i] = chess;
            }
            else if(temp >= 28 && temp <= 32){
                ChessGroup->AddNewObject(chess = new Chess("red_soldier.png", 635 + i * 200, 450 + j * 200, SOLDIER, RED));
                chessPositions[j][i] = chess;
            }
        }
        
    }


}
bool Edible(Chess* eater,Chess* prey){
    if(eater->getType()==SOLDIER&&prey->getType()==KING){
        //std::cout<<"1\n";
        return true;
    }
    else if(eater->getType()>=prey->getType()){
        //std::cout<<"1\n";
        return true;
    }
    else{
        return false;
    }
}



    

bool PlayScene::CannonValidEating(int j,int i,int temp_y,int temp_x){
    std::cout << j << " " << i << " " << temp_y << " " << temp_x << " " <<std::endl;
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
            if(chessPositions[j][k]!=nullptr)
                count++;
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
        int count = 0;
        for(int k=min+1;k<=max-1;k++){
            if(chessPositions[k][i]!=nullptr){
                count++;
            }
        }
    }


    if(count==1)
        return true;
    else
        return false;
}
void PlayScene::ConstructUI(){
    UIGroup->AddNewObject(UIRed = new Engine::Label(std::string("CD:")+ std::string(" 100%") ,"pirulen.ttf", 32, 1294, 520));
    UIGroup->AddNewObject(UIBlack = new Engine::Label(std::string("ATK:")+ std::string(" 100%"),"pirulen.ttf", 32, 1294, 720));
}

void PlayScene::InitializeRound(ChessColor color){
    turn = color==RED ? BLACK_TURN : RED_TURN;
    if(turn==RED_TURN)
        std::cout << "Red turn!\n";
    else
        std::cout << "Black turn!\n";
    //ConstructUI();
}

void PlayScene::ChangeRound(){
    turn = turn==RED_TURN ? BLACK_TURN : RED_TURN;
    if(turn==RED_TURN)
        std::cout << "Red turn!\n";
    else
        std::cout << "Black turn!\n";
    
}

void PlayScene::RecordTemp(Chess* chess,int i,int j){
    chess->Click();
    temp = chess;
    temp_x = i;
    temp_y = j;
}


void PlayScene::Move(Engine::Point pos,int j,int i){                         
    temp->Move(pos);
    chessPositions[j][i] = temp;
    chessPositions[temp_y][temp_x] = nullptr;
    temp->ClickCancel();
    temp = nullptr;
    ChangeRound();
}

void PlayScene::Update(float deltaTime) {

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
                pos.y = 450+j*200;
            }
            if(chess && ClickCheck(chess->Position)&& mouseJustClicked){
                if(!chess->Check_open()){
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
                                pos = chess->Position;
                                ChessGroup->RemoveObject(chess->GetObjectIterator());
                                Move(pos,j,i);
                            }
                        }
                        else if(abs(temp_y-j)+abs(temp_x-i)==1){//diff color
                            if(Edible(temp,chess)){
                                pos = chess->Position;
                                ChessGroup->RemoveObject(chess->GetObjectIterator());
                                Move(pos,j,i);
                            }
                               
                            else//can not eat
                                std::cout << "invalid!\n";
                        }
                        else
                            std::cout << "invalid!\n";
                    }
                    else{//have not select
                        color = chess->getColor();
                        if(color==RED&&turn==RED_TURN||color==BLACK&&turn==BLACK_TURN)
                            RecordTemp(chess,i,j);
                        else
                            std::cout << "Invalid Selection!\n";
                    }
                }
            }
            else if((!chess)&&ClickCheck(pos)&&temp&&mouseJustClicked){
                if(abs(temp_y-j)+abs(temp_x-i)==1){
                    Move(pos,j,i);//move
                }
            }
        }  
    }

}






void PlayScene::Terminate() {
    IScene::Terminate();
}

bool PlayScene::ClickCheck(Engine::Point pos){
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
