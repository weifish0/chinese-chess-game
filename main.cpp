// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/anqi_start_scene.hpp"
#include "Scene/mode_select.hpp"
#include "Scene/anqi_scene.hpp"
#include "Scene/red_win_scene.hpp"
#include "Scene/black_win_scene.hpp"
#include "Scene/harmony_scene.hpp"
#include "Scene/start_scene.hpp"
#include "Scene/xiangqi_scene.hpp"
#include "Scene/xiangqi_win_scene.hpp"
#include "Scene/playground.hpp"
#include "Scene/login.hpp"
#include "Scene/playground_setting_scene.hpp"
#include "Scene/anqi_setting_scene.hpp"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("playground_setting", new PlaygroundSettingScene());
	game.AddNewScene("anqi_setting", new AnqiSettingScene());
    game.AddNewScene("anqi_start", new Anqi_StartScene());
	game.AddNewScene("mode_select", new ModeSelectScene());
	game.AddNewScene("play", new AnqiScene());
	game.AddNewScene("red_win", new Red_Win_Scene());
	game.AddNewScene("black_win", new Black_Win_Scene());
	game.AddNewScene("harmony",new HarmonyScene());
    game.AddNewScene("start", new StartScene());
    game.AddNewScene("playground", new Playground());
    game.AddNewScene("login", new Login());

	game.AddNewScene("xiangqi", new XiangqiScene()); // Used for xiangqi branch development
	game.AddNewScene("xiangqi_win", new XiangqiWinScene()); // Used when a xiangqi game ends.

	game.Start("start", 60, 2666, 1500);
	
	return 0;
}
