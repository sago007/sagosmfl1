#include "GameConsole.hpp"
#include <iostream>

using namespace std;

struct GameConsole::GameConsoleImpl {
	const sago::SagoDataHolder *dataHolder;
	bool active = true;
};

GameConsole::GameConsole(const sago::SagoDataHolder &dataHolder) {
	data = new GameConsoleImpl();
	data->dataHolder = &dataHolder;
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
	sf::RectangleShape background(sf::Vector2f(sizex, sizey/2));
	target.draw(background);
}

void GameConsole::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	ReadEvents(input);
}

void GameConsole::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}

void GameConsole::ReadEvents(const sago::SagoCommandQueue &cmdQ) {
	
}