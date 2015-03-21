/* 
 * File:   World.cpp
 * Author: poul
 * 
 * Created on 14. december 2014, 11:46
 */

#include "World.hpp"

World::World() {
}

void World::SetSizeX(long long size) {
	this->sizeX = size;
}

long long World::GetSizeX() const {
	return sizeX;
}

void World::SetSizeY(long long sizeY) {
	this->sizeY = sizeY;
}

long long World::GetSizeY() const {
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
int World::GetTile (long long x, long long y) {
	if (x < 0 ||  y < 0) {
		return 0;
	}
	std::pair<long long, long long> location = {x / 50, y / 50};
	return (this->worldParts[location]).GetTile(x%50, y%50);
}

void World::SetTile(long long x, long long y, int tile) {
	std::pair<long long, long long> location = {x / 50, y / 50};
	(this->worldParts[location]).SetTile( x%50, y%50, tile);
}

void World::MakeExample() {
	for (long long x = 0; x < GetSizeX(); x ++) {
		for ( long long y = 0; y < GetSizeY(); y++) {
			if (x > 10 && x < 20 && y > 5 && y <25) { 
				SetTile(x, y, tileManager->GetId("clay"));
			}
			else {
				SetTile(x, y, tileManager->GetId("grass"));
			}
		}
	}
}