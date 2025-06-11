#ifndef NPC_HPP
#define NPC_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <string>
#include <vector>
#include <memory>

class NPC {
private:
    float x;
    float y;
    float size;
    std::shared_ptr<ALLEGRO_BITMAP> image;
    std::vector<std::string> dialogues;  // 對話內容列表
    size_t current_dialogue_index;       // 當前對話索引
    bool is_talking;                     // 是否正在對話
    static constexpr float INTERACTION_DISTANCE = 100.0f;  // 互動距離

public:
    NPC(float x, float y, float size, std::shared_ptr<ALLEGRO_BITMAP> img);
    ~NPC();

    // 基本屬性
    float getX() const { return x; }
    float getY() const { return y; }
    float getSize() const { return size; }
    bool getIsTalking() const { return is_talking; }

    // 對話相關
    void addDialogue(const std::string& dialogue);
    void startDialogue();
    void nextDialogue();
    void endDialogue();
    std::string getCurrentDialogue() const;
    bool hasMoreDialogue() const;

    // 互動檢測
    bool isInRange(float playerX, float playerY) const;

    // 繪製
    void Draw(float draw_x, float draw_y, float scale_x, float scale_y) const;
};

#endif // NPC_HPP 