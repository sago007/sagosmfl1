#include "GameConsole.hpp"
#include "sago/SagoMisc.hpp"
#include <iostream>
#include <list>

using namespace std;

struct GameConsole::GameConsoleImpl {
	const sago::SagoDataHolder *dataHolder;
	sago::GameStateManager *stateManager;
	bool active = true;
	sf::String currentBuffer;
	size_t cursorPos = 0;
	sf::Text text;
	sf::RectangleShape background;
	list<sf::String> history;
};

GameConsole::GameConsole(const sago::SagoDataHolder &dataHolder, sago::GameStateManager& stateManager) {
	data = new GameConsoleImpl();
	data->dataHolder = &dataHolder;
	data->stateManager = &stateManager;
	data->text.setFont(*dataHolder.getFontPtr("FreeSerif"));
	data->text.setString(data->currentBuffer);
	data->text.setCharacterSize(20);
	data->text.setColor(sf::Color::White);
	data->background.setFillColor(sf::Color(0,0,0,200));
	cerr << "Created console" << endl;
}

GameConsole::~GameConsole() {
	delete data;
}

bool GameConsole::IsActive() {
	return data->active;
}

bool GameConsole::IsBlockingDraw()  {
	return false;
}

bool GameConsole::IsBlockingUpdate()  {
	return false;
}

void GameConsole::Draw(sf::RenderWindow &target) {
	unsigned int sizex = target.getSize().x;
	unsigned int sizey = target.getSize().y;
	data->background.setSize(sf::Vector2f(sizex, sizey/2));
	target.draw(data->background);
	data->text.setString(data->currentBuffer);
	data->text.setPosition(0, sizey/2.0-data->text.getCharacterSize()*1.5-1.0);
	target.draw(data->text);
}

void GameConsole::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	ReadEvents(input);
}

void GameConsole::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}

static void InsertChar (sf::String& text, size_t& pos, sf::Uint32 newChar) {
	if (pos > text.getSize()) {
		return;
	}
	if (newChar == 8 /* backspace */) {
		if (pos > 0) {
			pos--;
			text.erase(pos,1);
		}
		return;
	}
	if (newChar == 127 /* delete */) {
		if (text.getSize() > pos) {
			text.erase(pos,1);
		}
		return;
	}
	if (newChar < 0x20) {
		//All other control charecters
		return;
	}
	text.insert(pos, newChar);
	pos++;	
} 

void GameConsole::ProcessCommand(const std::string& line) {
	vector<std::string> arg;
	arg.push_back(line);
	bool processed = data->stateManager->ProcessConsoleCommand(arg);
	if (!processed) {
		cerr << "Failed to process: " << line << endl;
	}
}

void GameConsole::ReadEvents(const sago::SagoCommandQueue &cmdQ) {
	sf::String inText = cmdQ.PeakText();
	for (sf::Uint32 item : inText) {
		if (item == 13 /* carriage return*/) {
			string outputText;
			sago::Utf32ToUtf8(data->currentBuffer, outputText);
			cout << outputText << endl;
			ProcessCommand(outputText);
			data->currentBuffer.clear();
			data->cursorPos = 0;
		}
		else {
			InsertChar(data->currentBuffer, data->cursorPos, item);
		}
	}
}