#include "npc.hpp"
#include <cmath>

NPC::NPC(float x, float y, float size, std::shared_ptr<ALLEGRO_BITMAP> img)
    : x(x), y(y), size(size), image(img), current_dialogue_index(0), is_talking(false) {
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
            draw_x, draw_y, size * scale_x, size * scale_y * 2,
            0
        );
    }

    // 如果正在對話，繪製對話框
    if (is_talking) {
        std::string current_dialogue = getCurrentDialogue();
        if (!current_dialogue.empty()) {
            // 這裡需要一個字體來繪製文字
            // 假設我們已經有一個字體變數 font
            // al_draw_text(font, al_map_rgb(0, 0, 0), draw_x, draw_y - 30, ALLEGRO_ALIGN_CENTER, current_dialogue.c_str());
        }
    }
} 