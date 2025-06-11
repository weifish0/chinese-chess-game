// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/start_scene.hpp"
#include "Scene/xiangqi_scene.hpp"
#include "Scene/xiangqi_win_scene.hpp"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

	// Register the scenes here!
    game.AddNewScene("start", new StartScene()); // Used for normal gameplay
	game.AddNewScene("xiangqi", new XiangqiScene()); // Used for xiangqi branch development
	game.AddNewScene("xiangqi_win", new XiangqiWinScene()); // Used when a xiangqi game ends.

	// game.Start("start", 60, 1600, 832); // Used for normal gameplay
	game.Start("xiangqi", 60, 1600, 832);
	
	return 0;
}
