// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/start_scene.hpp"
#include "Scene/playground.hpp"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    game.AddNewScene("start", new StartScene());
    game.AddNewScene("playground", new Playground());

	game.Start("start", 60, 2666, 1500);
	
	return 0;
}
