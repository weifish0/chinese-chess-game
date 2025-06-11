#ifndef XIANGQIWINSCENE_HPP
#define XIANGQIWINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <map>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/AudioHelper.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "ChessPiece/ChessPiece.hpp"
#include "UI/Component/Label.hpp"

#include "xiangqi_scene.hpp"

extern PieceColor winner;

// Engine::Collider collider; // Collider in XiangqiScene.
//                            // Due to the constructor of the collider, it is deemed as a non-static, const variable.
//                            // Hence not allowed to put inside XiangqiScene.

class XiangqiWinScene final : public Engine::IScene {
private:
public:
    /* PROPERTIES */

    /* METHODS */
    explicit XiangqiWinScene() = default;
    void Initialize() override;
    void ReadChessboard();
    void Update(float deltaTime) override;
    void Terminate() override;
};
#endif   // XIANGQIWINSCENE_HPP