/* 
 * File:   TileManager.hpp
 * Author: poul
 *
 * Created on 18. december 2014, 19:46
 */

#ifndef TILEMANAGER_HPP
#define	TILEMANAGER_HPP

#include "Items.hpp"
#include <vector>
#include <map>


class TileManager {
public:
	TileManager();
	const Tile& GetTile(int id) const;
	void AddTile(const Tile &tile);
	int GetId(const std::string &name);
private:
	std::vector<Tile> tiles;
	std::map<std::string,int> idMap;
};

#endif	/* TILEMANAGER_HPP */

