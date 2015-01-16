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

int World::GetTile (long long x, long long y) const {
	if (x < 0 || y < 0 || x > GetSizeX() || y > GetSizeY()) {
		return 0;
	}
	if (x > 10 && x < 20 && y > 5 && y <25) {
		return tileManager->GetId("clay");
	}
	return tileManager->GetId("grass");
}

