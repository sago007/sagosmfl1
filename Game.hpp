/* 
 * File:   game.hpp
 * Author: poul
 *
 * Created on 29. november 2014, 14:36
 */

#ifndef GAME_HPP
#define	GAME_HPP

#include "sago/GameState.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoDataHolder.hpp"
#include "model/World.hpp"

class Game : public sago::GameState {
public:
	Game(const sago::SagoDataHolder &dataHolder);
	~Game();
	bool IsActive() override;
	bool IsBlockingDraw() override;
	bool IsBlockingUpdate() override;
	void Draw(sf::RenderWindow &target) override;
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input) override;
	void UpdateCommandQueue(sago::SagoCommandQueue &inout) override;
	bool ProcessConsoleCommand(const std::vector<std::string>& arg) override;
private:
	void DrawTiles(sf::RenderWindow &target, int topX, int topY, int width, int height, World& world, long long worldX, long long worldY, int tileSize, int mouseTileX, int mouseTileY);
	struct GameImpl;
	GameImpl* data;
};

#endif	/* GAME_HPP */

