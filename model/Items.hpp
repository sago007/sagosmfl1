/* 
 * File:   items.hpp
 * Author: poul
 *
 * Created on 2. december 2014, 17:47
 */

#ifndef ITEMS_HPP
#define	ITEMS_HPP

#include <string>

class Tile {
public:
	Tile() = default;
	Tile(unsigned int id, std::string name, std::string sprite, bool blocking) : internalId(id), name(name), sprite(sprite), blocking(blocking) {};
	unsigned int internalId = 0;
	std::string name;
	std::string sprite;
	bool blocking = false;
};

class Placeable {
public:
	float X = 10.0;
	float Y = 20.0;
	float Radius = 0.0;
	virtual bool isStatic() {return true; }
};

class MistItem : public Placeable {
public:
	std::string sprite;
};

class Creature : public Placeable {
public:
	float health = 10.0;
	float stinema = 10.0;
	float mana = 10.0;
	char direction = 'S';
	bool moving = false;
	virtual bool isStatic() override { return false; }
};

class Human : public Creature {
public:
	std::string race = "male";
};

#endif	/* ITEMS_HPP */

