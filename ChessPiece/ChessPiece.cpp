#include "ChessPiece/ChessPiece.hpp"

#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

float blockSize = (float) 1000 / (float) 13; // <blockSize = Screen.y / 13>
                                             // Follow the same pattern if you want to change block size.

ChessPiece::ChessPiece(std::string img, Engine::Point position, PieceColor color, int score) 
    : Sprite(img, position.x, position.y, 90, 90), color(color), score(score) {
}
ChessPiece *ChessPiece::Clone() const{
    std::cout << "[LOG] ChessPiece Clone!" << std::endl;
    return new ChessPiece(*this); // Default copy behavior (@chatgpt)
}
bool ChessPiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState) {
    return true;
}
bool ChessPiece::IsCheckmate(std::vector<std::vector<std::pair<int,ChessPiece*>>> &ChessboardState) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    int row = std::floor((Position.x + 0.5 * blockSize - halfW) / blockSize) + 4; // The row of the current king piece.
    int col = std::floor((Position.y - halfH) / blockSize) + 5;                   // The column of the current king piece.
    std::cout << "(" << row << "," << col << "): ";

    if (abs(ChessboardState[row][col].first) != WANG) return false; // No access by other types of pieces. (except KING!)

    // Traverse through all the pieces (smartly).
    int r; // The row of the traversed piece.
    int c; // The column of the traversed piece.
    for (int r = 0; r < ChessboardState.size(); r++) {
        std::cout << "[LOG] ChessboardState.size() = " << ChessboardState.size() << std::endl;//
        for (int c = 0; c < ChessboardState[r].size(); c++) {
            std::cout << "[LOG] ChessboardState[r].size() = " << ChessboardState[r].size() << std::endl;//
            auto state = ChessboardState[r][c].first;
            auto piece = ChessboardState[r][c].second;

            if (abs(state) == NONE || abs(state) == GUARD || abs(state) == ELFNT || state * color > 0) // Ignore PieceType NONE, GUARD and ELFNT, and ignore those of the same color.
                continue;

            if (piece->IsValidMove(r, c, row, col, ChessboardState)) { // If it is possible for the traversed piece to attack itself:
                return true;
            }
        }
    }

    return false;
}

void ChessPiece::OnAnimation(ChessPiece *enemy_piece) {
}

bool ChessPiece::IsInPalace(int row, int col) {
    if (color == HONG) {
        if (7 <= row && row <= 9 && 3 <= col && col <= 5) return true;
        else return false;

    } else if (color == HEI) {
        if (0 <= row && row <= 2 && 3 <= col && col <= 5) return true;
        else return false;
    }

    // Default
    return false;
}