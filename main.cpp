// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/start_scene.hpp"
#include "Scene/mode_select.hpp"
#include "Scene/play_scene.hpp"
#include "Scene/red_win_scene.hpp"
#include "Scene/black_win_scene.hpp"
#include "Scene/harmony_scene.hpp"
int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    game.AddNewScene("start", new StartScene());
	game.AddNewScene("mode_select", new ModeSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("red_win", new Red_Win_Scene());
	game.AddNewScene("black_win", new Black_Win_Scene());
	game.AddNewScene("harmony",new HarmonyScene());
	//game.AddNewScene("setting", new SettingScene());

	game.Start("start", 60, 2666, 1500);
	
	return 0;
}
