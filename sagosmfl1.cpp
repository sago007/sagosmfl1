#include <iostream>
#include <boost/program_options.hpp>
#include "sago/GameStateManager.hpp"
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoMisc.hpp"
#include "sago/SagoCommandQueue.hpp"
#include "sago/SagoSprite.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include "sago/FrameCounter.hpp"
#include "sago/SagoMusicBackground.hpp"
#include <SFML/Graphics.hpp>
#include <physfs.h>
#include "MainMenu.hpp"
#include "CommandProcessor.hpp"

using namespace std;

namespace {
	void SetStandardKeyBinds(sago::SagoCommandQueue &cmdQ) {
		cmdQ.BindKey(sf::Keyboard::Up,"UP");
		cmdQ.BindKeyCommand("UP","UP");
		cmdQ.BindKey(sf::Keyboard::Down,"DOWN");
		cmdQ.BindKeyCommand("DOWN","DOWN");
		cmdQ.BindKey(sf::Keyboard::Left,"LEFT");
		cmdQ.BindKey(sf::Keyboard::Right,"RIGHT");
		cmdQ.BindKey(sf::Keyboard::Return,"RETURN");
		cmdQ.BindKeyCommand("RETURN","CONFIRM");
		cmdQ.BindKey(sf::Keyboard::Escape,"ESC");
		cmdQ.BindKeyCommand("ESC","BACK");
		cmdQ.BindKey(sf::Keyboard::F2, "UNDER-ESC");
		cmdQ.BindKeyCommand("UNDER-ESC","CONSOLE_TOGGLE");
	}
	
	void ArgAndPhysInit(int argc, const char* argv[]) {
		boost::program_options::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "Print basic usage information to stdout and quits")
			("somestring", boost::program_options::value<string>(), "A string to print")
		;
		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
		boost::program_options::notify(vm);
		if (vm.count("help")) {
			cout << desc << endl;
			exit(1);
		}
		PHYSFS_init(argv[0]);
		std::string baseSearchPath = PHYSFS_getBaseDir();
		baseSearchPath += "/data/";
		PHYSFS_addToSearchPath(baseSearchPath.c_str(), 0); std::string baseWritePath = PHYSFS_getBaseDir();
		int errorCode = PHYSFS_setWriteDir(baseWritePath.c_str());
		if (errorCode == 0) {
			cerr <<  PHYSFS_getLastError() << endl;
		}
	}
	
	struct BaseItems {
		unique_ptr<sago::SagoDataHolder> dataHolder;
		unique_ptr<sago::SagoSpriteHolder> spriteHolder;
		unique_ptr<sago::SagoCommandQueue> cmdQ;
		unique_ptr<sago::FrameCounter> fc;
		
		BaseItems() {
			dataHolder = unique_ptr<sago::SagoDataHolder> (new sago::SagoDataHolder());
			spriteHolder = unique_ptr<sago::SagoSpriteHolder> (new sago::SagoSpriteHolder(*dataHolder));
			cmdQ = unique_ptr<sago::SagoCommandQueue> (new sago::SagoCommandQueue());
			SetStandardKeyBinds(*cmdQ);
			fc = unique_ptr<sago::FrameCounter>( new sago::FrameCounter(*dataHolder));
		}
	};
}

int main(int argc, const char* argv[])
{
	ArgAndPhysInit(argc,argv);
	BaseItems base;
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Sago test 1");
	sago::GameStateManager stateManager;
	std::shared_ptr<sago::GameState> menu(new MainMenu(*base.dataHolder));
	stateManager.PushState(menu);
	sf::Clock clock;  //start the clock
	sf::Int32 lastFrameTime = 0;
	while (window.isOpen()) {
		sf::Int32 frameTime = clock.getElapsedTime().asMilliseconds();
		sf::Int32 deltaTime = frameTime - lastFrameTime;
		float fDeltaTime = static_cast<float>(deltaTime);
		lastFrameTime = frameTime;
		base.cmdQ->ReadKeysAndAddCommands(window);
		if (base.cmdQ->Closing()) {
			window.close();
		}
		ProcessCommands(*base.cmdQ,*base.dataHolder,stateManager);
		stateManager.Update(fDeltaTime,*base.cmdQ);
		base.cmdQ->ClearCommands();
		stateManager.UpdateCommandQueue(*base.cmdQ);
		for (const std::string &cmd : base.cmdQ->GetCommandQueue()) {
			if (cmd == "QUIT") {
				window.close();
			}
		}
		window.clear();
		stateManager.Draw(window);
		base.fc->Draw(window,frameTime);
		window.display();
		usleep(100);
	}
	PHYSFS_deinit();
	return 0;
}
