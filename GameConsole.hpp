#ifndef GAMECONSOLE_HPP
#define	GAMECONSOLE_HPP

#include "sago/GameState.hpp"
#include "sago/SagoDataHolder.hpp"

class GameConsole : public sago::GameState {
public:
	GameConsole(const sago::SagoDataHolder &dataHolder);
	~GameConsole();
	bool IsActive() override;
	bool IsBlockingDraw() override;
	bool IsBlockingUpdate() override;
	void Draw(sf::RenderWindow &target) override;
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input) override;
	void UpdateCommandQueue(sago::SagoCommandQueue &inout) override;
private:
	void ReadEvents(const sago::SagoCommandQueue &cmdQ);
	struct GameConsoleImpl;
	GameConsoleImpl* data;
};


#endif  // GAMECONSOLE_HPP
