/* 
 * File:   Store.hpp
 * Author: poul
 *
 * Created on 29. december 2014, 17:38
 */

#ifndef STORE_HPP
#define	STORE_HPP

#include "Items.hpp"
#include <vector>
#include <cppdb/frontend.h>

class Store {
public:
	Store();
	Store(const Store& orig) = delete;
	void Open(const std::string& filename);
	std::vector<Tile> GetTiles();
	virtual ~Store();
private:
	long long int sizeX = 0;
	long long int sizeY = 0;
	unsigned int baseTile = 0;
	std::unique_ptr<cppdb::session> sql;
};

#endif	/* STORE_HPP */

