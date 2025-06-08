#ifndef XIANGQISCENE_HPP
#define XIANGQISCENE_HPP
#include <allegro5/allegro_audio.h>
#include <map>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"

class XiangqiScene final : public Engine::IScene {
public:
    /* PROPERTIES */
    Engine::Group *PieceGroup; // Pieces on chessboard. 場上的棋子們。

    /* METHODS */
    explicit XiangqiScene() = default;
    void Initialize() override;
    // void Update();
};
#endif   // XIANGQISCENE_HPP
