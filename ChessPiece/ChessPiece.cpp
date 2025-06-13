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

ChessPiece::ChessPiece(std::string img, Engine::Point position, PieceColor country, int score) 
    : Sprite(img, position.x, position.y, 90, 90), country(country), score(score) {
}
ChessPiece *ChessPiece::Clone() const{
    std::cout << "[LOG] ChessPiece Clone!" << std::endl;
    return new ChessPiece(*this); // Default copy behavior (@chatgpt)
}
bool ChessPiece::IsValidMove(int row, int col, int next_row, int next_col, std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard) {
    return true;
}
bool ChessPiece::IsCheckmate(std::vector<std::vector<std::pair<int,ChessPiece*>>> &Chessboard) {
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int halfH = h / 2;

    int col = std::floor((Position.x + 0.5 * blockSize - halfW) / blockSize) + 4; // The row of the current king piece.
    int row = std::floor((Position.y - halfH) / blockSize) + 5;                   // The column of the current king piece.
    std::cout << "(" << row << "," << col << "): ";

    if (abs(Chessboard[row][col].first) != WANG) return false; // No access by other types of pieces. (except KING!)

    // Traverse through all the pieces (smartly).
    int r; // The row of the traversed piece.
    int c; // The column of the traversed piece.
    for (int r = 0; r < Chessboard.size(); r++) {
        for (int c = 0; c < Chessboard[r].size(); c++) {
            auto state = Chessboard[r][c].first;
            auto piece = Chessboard[r][c].second;

            if (abs(state) == NONE || abs(state) == GUARD || abs(state) == ELFNT || state * country > 0) // Ignore PieceType NONE, GUARD and ELFNT, and ignore those of the same country.
                continue;

            if (piece->IsValidMove(r, c, row, col, Chessboard)) { // If it is possible for the traversed piece to attack itself:
                return true;
            }
        }
    }

    return false;
}

int ChessPiece::GeneralFlies(int red_row, int red_col, int black_row, int black_col, std::vector<std::vector<std::pair<int, ChessPiece*>>> &Chessboard, PieceColor Round) {
    // Only RedKing can be accessed, and only BlackKing can be tested.
    if (Chessboard[red_row][red_col].first != HONG * WANG || Chessboard[black_row][black_col].first != HEI * WANG)
        return 0;
    
    if (red_col == black_col) { // The king of another country sharing the same column.
        int min_r, max_r;
        min_r = red_row < black_row ? red_row : black_row;
        max_r = red_row > black_row ? red_row : black_row;
        for (int r = min_r + 1; r < max_r; r++) {
            if (Chessboard[r][red_col].second != nullptr) { // There IS a piece in between.
                return 0; // Not "Flying General"
            }
        }

        // General does "fly"... But which one?
        return Round; // If the current Round is HONG, it means that HONG induced the execution of flying general. SO GENERAL OF COLOR HEI FLIES.
                      // Else, if Round == HEI, it means that HEI induced the execution of flying general. THUS GENERAL OF COLOR HONG FLIES.

    } else {
        return 0;
    }
}

void ChessPiece::OnAnimation(ChessPiece *enemy_piece) {
}

bool ChessPiece::IsInPalace(int row, int col) {
    if (country == HONG) {
        if (7 <= row && row <= 9 && 3 <= col && col <= 5) return true;
        else return false;

    } else if (country == HEI) {
        if (0 <= row && row <= 2 && 3 <= col && col <= 5) return true;
        else return false;
    }

    // Default
    return false;
}