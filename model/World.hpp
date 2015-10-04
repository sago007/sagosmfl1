/* 
 * File:   World.hpp
 * Author: poul
 *
 * Created on 14. december 2014, 11:46
 */

#ifndef WORLD_HPP
#define	WORLD_HPP

#include "TileManager.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/utility.hpp"


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
	
	int GetTile (int x, int y);
	void SetTile ( int x, int y, int tile);
    void SetSizeX(int size);
    int GetSizeX() const;
    void SetSizeY(int sizeY);
    int GetSizeY() const;
    void SetTileManager(std::shared_ptr<TileManager> tileManager);
	const std::shared_ptr<TileManager>& GetTileManager() const; 
//private:
	int sizeX = 1000;
	int sizeY = 1000;
	std::shared_ptr<TileManager> tileManager;
	std::map<std::pair<int, int>, WorldPart> worldParts;
};

namespace cereal {
	
template<class Archive>
void save(Archive & archive, WorldPart const & m) {
	archive( cereal::make_nvp("Tiles", m.tiles) );
}

template<class Archive>
void load(Archive & archive, WorldPart & m) {
	archive( cereal::make_nvp("Tiles", m.tiles) );
}

template<class Archive>
void save(Archive & archive, World const & m) {
	archive( cereal::make_nvp("TileManager", *(m.tileManager.get())), cereal::make_nvp("sizeX", m.sizeX), cereal::make_nvp("sizeY", m.sizeY),  cereal::make_nvp("WorldParts", m.worldParts) );
}

/**
 * This template function loads from an archive into a World.
 * World must already have the TileManager set because the tranforming is done using that.
 * @param archive
 * @param m
 */
template<class Archive>
void load(Archive & archive, World & m) {
	TileManager tileM;
	archive >> cereal::make_nvp("TileManager", tileM);
	std::map<int, int> tileMapperForLoading = m.tileManager->getMapperFrom(tileM);
	archive( cereal::make_nvp("sizeX", m.sizeX), cereal::make_nvp("sizeY", m.sizeY),  cereal::make_nvp("WorldParts", m.worldParts) );
	for (auto& parts : m.worldParts) {
		for (int& tile : parts.second.tiles) {
			tile = tileMapperForLoading[tile];
		}
	}
}
	
}

#endif	/* WORLD_HPP */


