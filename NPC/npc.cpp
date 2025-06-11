#include "npc.hpp"
#include "Engine/Resources.hpp"
#include <allegro5/allegro_primitives.h>
#include <cmath>

NPC::NPC(float x, float y, float size, std::shared_ptr<ALLEGRO_BITMAP> img)
    : x(x), y(y), size(size), image(img), current_dialogue_index(0), is_talking(false) {
    // 載入字體
    font = Engine::Resources::GetInstance().GetFont("font2.ttc", DIALOGUE_FONT_SIZE);
}

NPC::~NPC() {
}

void NPC::addDialogue(const std::string& dialogue) {
    dialogues.push_back(dialogue);
}

void NPC::startDialogue() {
    if (!dialogues.empty()) {
        is_talking = true;
        current_dialogue_index = 0;
    }
}

void NPC::nextDialogue() {
    if (is_talking && current_dialogue_index < dialogues.size() - 1) {
        current_dialogue_index++;
    } else {
        endDialogue();
    }
}

void NPC::endDialogue() {
    is_talking = false;
    current_dialogue_index = 0;
}

std::string NPC::getCurrentDialogue() const {
    if (is_talking && current_dialogue_index < dialogues.size()) {
        return dialogues[current_dialogue_index];
    }
    return "";
}

bool NPC::hasMoreDialogue() const {
    return is_talking && current_dialogue_index < dialogues.size() - 1;
}

bool NPC::isInRange(float playerX, float playerY) const {
    float dx = playerX - x;
    float dy = playerY - y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= INTERACTION_DISTANCE;
}

void NPC::Draw(float draw_x, float draw_y, float scale_x, float scale_y) const {
    // 繪製 NPC 圖片
    if (image) {
        al_draw_scaled_bitmap(
            image.get(),
            0, 0, al_get_bitmap_width(image.get()), al_get_bitmap_height(image.get()),
            draw_x, draw_y, size * scale_x, size * scale_y * 2, // 圖片大小 64*128
            0
        );
    }

    // 如果正在對話，繪製對話框和文字
    if (is_talking && font) {
        std::string current_dialogue = getCurrentDialogue();
        if (!current_dialogue.empty()) {
            // 繪製對話框背景
            float text_width = al_get_text_width(font.get(), current_dialogue.c_str());
            float text_height = al_get_font_line_height(font.get());
            float padding = 10.0f;
            
            // 對話框位置（在 NPC 上方）
            float box_x = draw_x - text_width/2 - padding;
            float box_y = draw_y - text_height - padding - 20;  // 20 是與 NPC 的距離
            
            // 繪製對話框背景
            al_draw_filled_rectangle(
                box_x, box_y,
                box_x + text_width + padding * 2,
                box_y + text_height + padding * 2,
                al_map_rgba(255, 255, 255, 200)  // 半透明白色背景
            );
            
            // 繪製對話框邊框
            al_draw_rectangle(
                box_x, box_y,
                box_x + text_width + padding * 2,
                box_y + text_height + padding * 2,
                al_map_rgb(0, 0, 0),  // 黑色邊框
                2.0f  // 邊框寬度
            );
            
            // 繪製文字
            al_draw_text(
                font.get(),
                al_map_rgb(0, 0, 0),  // 黑色文字
                draw_x,
                box_y + padding,
                ALLEGRO_ALIGN_CENTER,
                current_dialogue.c_str()
            );
        }
    }
} 