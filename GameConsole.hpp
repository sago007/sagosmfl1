#ifndef GAMECONSOLE_HPP
#define	GAMECONSOLE_HPP

#include "sago/GameState.hpp"
#include "sago/SagoDataHolder.hpp"
#include "sago/GameStateManager.hpp"

class GameConsole : public sago::GameState {
public:
	GameConsole(const sago::SagoDataHolder &dataHolder, sago::GameStateManager& stateManager);
	~GameConsole();
	bool IsActive() override;
	bool IsBlockingDraw() override;
	bool IsBlockingUpdate() override;
	void Draw(sf::RenderWindow &target) override;
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input) override;
private:
	void HistoryGetAt(int index, sf::String& output);
	void HistoryPush(const sf::String &input);
	void HistoryUp();
	void HistoryDown();
	void ReadEvents(const sago::SagoCommandQueue &cmdQ);
	void ProcessCommand(const std::string& line);
	struct GameConsoleImpl;
	GameConsoleImpl* data;
};


#endif  // GAMECONSOLE_HPP
