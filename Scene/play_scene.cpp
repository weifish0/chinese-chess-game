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

void PlayScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    al_init_image_addon();

    ALLEGRO_BITMAP* image = al_load_bitmap("Resource/images/chess_board.png");
    if (!image) {
        std::cerr << "Failed to load image!" << std::endl;
    }
    al_draw_bitmap(image, 0, 0, 0); // x, y 是圖片顯示的位置




}
void PlayScene::Terminate() {
    IScene::Terminate();
}
