/* 
 * File:   TileManager.cpp
 * Author: poul
 * 
 * Created on 18. december 2014, 19:46
 */


#include "TileManager.hpp"

TileManager::TileManager() {
}

const Tile& TileManager::GetTile(int id) const {
	return tiles.at(id);
}

void TileManager::AddTile(const Tile &tile) {
	unsigned int newId = tile.internalId;
	if (newId >= tiles.size()) {
		tiles.resize(newId+1);
	}
	tiles.at(newId) = tile;
	idMap[tile.name] = tile.internalId;
}

int TileManager::GetId(const std::string &name) {
	return idMap[name];
}