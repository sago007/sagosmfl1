/* 
 * File:   World.hpp
 * Author: poul
 *
 * Created on 14. december 2014, 11:46
 */

#ifndef WORLD_HPP
#define	WORLD_HPP

#include "TileManager.hpp"

class World {
public:
	World();
	World(const World& orig);
	virtual ~World();
	
	int GetTile (long long x, long long y) const;
    void SetSizeX(long long size);
    long long GetSizeX() const;
    void SetSizeY(long long sizeY);
    long long GetSizeY() const;
private:
	long long sizeX = 1000;
	long long sizeY = 1000;
};

#endif	/* WORLD_HPP */

