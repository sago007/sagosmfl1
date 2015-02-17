/* 
 * File:   World.hpp
 * Author: poul
 *
 * Created on 14. december 2014, 11:46
 */

#ifndef WORLD_HPP
#define	WORLD_HPP

#include "TileManager.hpp"

struct WorldPart {
	WorldPart() {
		tiles.assign(2500,0);
	}
	int GetTile(int x, int y) {
		return tiles.at(x*50+y);
	}
	void SetTile(int x, int y, int tile) {
		tiles.at(x*50+y) = tile;
	}
	std::vector<int> tiles;
};

class World {
public:
	World();
	World(const World& orig);
	virtual ~World();
	
	void MakeExample();
	
	int GetTile (long long x, long long y);
	void SetTile ( long long x, long long y, int tile);
    void SetSizeX(long long size);
    long long GetSizeX() const;
    void SetSizeY(long long sizeY);
    long long GetSizeY() const;
    void SetTileManager(std::shared_ptr<TileManager> tileManager);
	const std::shared_ptr<TileManager>& GetTileManager() const; 
private:
	long long sizeX = 1000;
	long long sizeY = 1000;
	std::shared_ptr<TileManager> tileManager;
	std::map<std::pair<long long, long long>, WorldPart> worldParts;
};
#endif	/* WORLD_HPP */


