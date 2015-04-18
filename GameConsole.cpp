#include "GameConsole.hpp"
#include "sago/SagoMisc.hpp"
#include <iostream>
#include <list>
#include <boost/tokenizer.hpp>

using namespace std;

struct GameConsole::GameConsoleImpl {
	const sago::SagoDataHolder *dataHolder;
	sago::GameStateManager *stateManager;
	bool active = true;
	sf::String currentBuffer;
	size_t cursorPos = 0;
	sf::Text text;
	sf::RectangleShape background;
	vector<sf::String> history;
	int historyTop = 0;
	int historySize = 30000;
	int historyPos = 0;
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
	if (data->history.size() < (size_t)data->historySize) {
		data->history.resize(data->historySize);
	}
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

void GameConsole::HistoryGetAt(int index, sf::String& output) {
	int realIndex = (index+data->historyTop)%data->historySize;
	if (realIndex < 0) {
		realIndex += data->historySize;
	}
	output = data->history.at(realIndex);
	
}
void GameConsole::HistoryPush(const sf::String &input) {
	if (data->history.at(data->historyTop) == input || input == "") {
		return; //data identical to top or blank
	}	
	data->historyTop++;
	if (data->historyTop >= data->historySize) {
		data->historyTop = 0;
	}
	data->history.at(data->historyTop) = input;
}


void GameConsole::HistoryUp() {
	if (data->historyPos == 0) {
		HistoryPush(data->currentBuffer);
	}
	data->historyPos--;
	HistoryGetAt(data->historyPos, data->currentBuffer);
	if (data->currentBuffer == "") {
		data->historyPos++;
		HistoryGetAt(data->historyPos, data->currentBuffer);
	}
}

void GameConsole::HistoryDown() {
	if (data->historyPos < 0) {
		data->historyPos++;
		HistoryGetAt(data->historyPos, data->currentBuffer);
	}
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
	boost::escaped_list_separator<char> els('\\',' ','\"');
	boost::tokenizer<boost::escaped_list_separator<char> > tok(line, els);
	for (const string& item : tok) {
		arg.push_back(item);
	}
	bool processed = false;
	try {
		processed = data->stateManager->ProcessConsoleCommand(arg);
	} catch (exception& e) {
		cerr << "std::exception: " << e.what() << endl;
	}
	catch (...) {
	}
	if (!processed) {
		cerr << "Failed to process: " << arg.at(0) ;
		for (size_t i = 1; i < arg.size(); ++i) {
			cerr << " \"" << arg.at(i) << "\"";
		}
		cerr << endl;
	}
}

void GameConsole::ReadEvents(const sago::SagoCommandQueue &cmdQ) {
	for (size_t i = 0; i < cmdQ.GetCommandQueue().size(); i++) {
			std::string cmd = cmdQ.GetCommandQueue().at(i);
		if (cmd == "UP") {
			HistoryUp();
			return;
		}
		if (cmd == "DOWN") {
			HistoryDown();
			return;
		}
	}
	sf::String inText = cmdQ.PeakText();
	for (sf::Uint32 item : inText) {
		if (item == 13 /* carriage return*/) {
			string outputText;
			sago::Utf32ToUtf8(data->currentBuffer, outputText);
			ProcessCommand(outputText);
			HistoryPush(outputText);
			data->currentBuffer.clear();
			data->cursorPos = 0;
		}
		else {
			InsertChar(data->currentBuffer, data->cursorPos, item);
		}
	}
}