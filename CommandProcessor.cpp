#include "CommandProcessor.hpp"
#include "Game.hpp"
#include "GameConsole.hpp"

using namespace std;


void ProcessCommands(sago::SagoCommandQueue &cmdQ, sago::SagoDataHolder &dh, sago::GameStateManager &m) {
	for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
		string cmd = cmdQ.GetCommandQueue().at(i);
		if (cmd == "START_GAME") {
			shared_ptr<sago::GameState> game(new Game(dh));
			m.PushState(game);
		}
		if (cmd == "CONSOLE_TOGGLE" ) {
			if (m.ConsoleOpen()) {
				m.SetConsole(nullptr);
			}
			else {
				shared_ptr<sago::GameState> console(new GameConsole(dh));
				m.SetConsole(console);
			}
		}
	}
}