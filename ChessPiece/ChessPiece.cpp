#include "ChessPiece/ChessPiece.hpp"

#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

ChessPiece::ChessPiece(std::string img, Engine::Point position, PieceColor color, bool isPreview, int score) 
    : Sprite(img, position.x, position.y, 90, 90), color(color), isPreview(isPreview), score(score) {
}
bool ChessPiece::IsValidMove(Engine::Point nextPos, std::vector<std::vector<int>> ChessboardState) {
    return true;
}
void ChessPiece::OnAnimation(ChessPiece *enemy_piece) {
}