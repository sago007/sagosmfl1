/* 
 * File:   World.cpp
 * Author: poul
 * 
 * Created on 14. december 2014, 11:46
 */

#include "World.hpp"

World::World() {
}

void World::SetSizeX(int size) {
	this->sizeX = size;
}

int World::GetSizeX() const {
	return sizeX;
}

void World::SetSizeY(int sizeY) {
	this->sizeY = sizeY;
}

int World::GetSizeY() const {
	return sizeY;
}

void World::SetTileManager(std::shared_ptr<TileManager> tileManager) {
	this->tileManager = tileManager;
}

const std::shared_ptr<TileManager>& World::GetTileManager() const {
	return this->tileManager;
}

World::World(const World& orig) {
	*this = orig;
}

World::~World() {
}

/**
 * Returns a file for a given coordinate
 * @note This is not const as it may load data from disk/database as needed
 * @param x X coordiante. Must be positive
 * @param y Y coordinate. Must be positive
 * @return The file
 */
int World::GetTile (int x, int y) {
	if (x < 0 ||  y < 0) {
		return 0;
	}
	std::pair<int, int> location = {x / 50, y / 50};
	return (this->worldParts[location]).GetTile(x%50, y%50);
}

void World::SetTile(int x, int y, int tile) {
	std::pair<int, int> location = {x / 50, y / 50};
	(this->worldParts[location]).SetTile( x%50, y%50, tile);
}

void World::MakeExample() {
	for (int x = 0; x < GetSizeX(); x ++) {
		for ( int y = 0; y < GetSizeY(); y++) {
			if (x > 10 && x < 20 && y > 5 && y <25) { 
				SetTile(x, y, tileManager->GetId("clay"));
			}
			else {
				SetTile(x, y, tileManager->GetId("grass"));
			}
		}
	}
}