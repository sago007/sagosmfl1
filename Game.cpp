/* 
 * File:   game.cpp
 * Author: poul
 * 
 * Created on 29. november 2014, 14:36
 */

#include "Game.hpp"
#include "model/Items.hpp"
#include "model/Misc.hpp"
#include "model/Store.hpp"
#include "model/TileManager.hpp"
#include "model/World.hpp"
#include <iostream>
#include <bits/shared_ptr.h>
#include "sago/SagoSprite.hpp"
#include "sago/SagoSpriteHolder.hpp"

using namespace std;

/**
 * This sort method sorts the elements from furthest to screen to closest to screen, so that elemnets closer to the screen will be drawn last
 * @param lhs Left hand side
 * @param rhs Right hand side
 * @return true if lhs < rhs
 */
static bool sort_placeable(const shared_ptr<Placeable> &lhs, const shared_ptr<Placeable> &rhs)
{
	return lhs->Y < rhs->Y;
}

struct Game::GameImpl {
	vector<shared_ptr<Placeable> > placeables;
	const sago::SagoDataHolder *dataHolder;
	std::shared_ptr<sago::SagoSpriteHolder> sprites;
	float time = 0.0;
	shared_ptr<Human> human;
	long long center_x = 0;
	long long center_y = 0;
	World mainworld;
	shared_ptr<TileManager> tileManager;
	Store store;
};

static void CreateTiles( TileManager &manager) {
	TileReadFile(manager, "basic.tiles");
}

Game::Game(const sago::SagoDataHolder &dataHolder) {
	data = new GameImpl();
	data->dataHolder = &dataHolder;
	data->sprites = std::shared_ptr<sago::SagoSpriteHolder>(new sago::SagoSpriteHolder(*(data->dataHolder)));
	data->tileManager = shared_ptr<TileManager>(new TileManager());
	data->mainworld.SetTileManager(data->tileManager);
	data->store.Open("test.db");
	shared_ptr<Human> human (new Human());
	human->Radius = 16.0f;
	data->placeables.push_back(human);
	data->human = human;
	CreateTiles(*(data->tileManager));
	data->mainworld.MakeExample();
	shared_ptr<MistItem> p(new MistItem());
	p->Radius = 16.0;
	p->X = 50.0;
	p->Y = 140.0;
	p->sprite = "item_barrel";
	data->placeables.push_back(p);
	shared_ptr<MistItem> p2(new MistItem());
	*p2 = *p;
	p2->X = 100.0;
	data->placeables.push_back(p2);
}

Game::~Game() {
	delete data;
}

bool Game::IsActive() {
	return true;
}
bool Game::IsBlockingDraw()  {
	return true;
}
bool Game::IsBlockingUpdate()  {
	return true;
}

static void CheckCollision(vector<shared_ptr<Placeable> > &placeables) {
	for (unsigned int i = 0; i < placeables.size(); ++i) {
		for (unsigned int j = i+1; j < placeables.size(); ++j) {
			if ( IsTouching(*placeables.at(i).get(), *placeables.at(j).get())) {
				cout << "Tocuhing" << endl;
			}
		}
	}
	
}

static void DrawHumanEntity(sf::RenderWindow &target, const std::shared_ptr<sago::SagoSpriteHolder> &sHolder, const Human *entity, float time, long long offsetX, long long offsetY) {
	string animation = "standing";
	if (entity->moving) {
		animation = "walkcycle";
	}
	sf::CircleShape circle(entity->Radius);
	circle.setPosition(entity->X-offsetX-16, entity->Y-offsetY-16);
	target.draw(circle);
	const sago::SagoSprite &mySprite = sHolder->GetSprite(entity->race + "_"+animation+"_"+string(1,entity->direction));
	mySprite.Draw(target, time, entity->X-offsetX, entity->Y-offsetY);
}

static void DrawMiscItem(sf::RenderWindow &target, const std::shared_ptr<sago::SagoSpriteHolder> &sHolder, const MistItem *entity, float time, long long offsetX, long long offsetY) {
	sf::CircleShape circle(entity->Radius);
	circle.setPosition(entity->X-offsetX-16, entity->Y-offsetY-16);
	target.draw(circle);
	const sago::SagoSprite &mySprite = sHolder->GetSprite(entity->sprite);
	mySprite.Draw(target, time, entity->X-offsetX, entity->Y-offsetY);
}

void Game::DrawTiles(sf::RenderWindow &target, int topXpixel, int topYpixel, int width, int height, World& world, long long worldX, long long worldY) {
	
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int tileId = world.GetTile(worldX+x, worldY+y);
			if ( tileId == 0) {
				continue;
			}
			const sago::SagoSprite &sprite = data->sprites->GetSprite(data->tileManager->GetTile(tileId).sprite);
			sprite.Draw(target, data->time, topXpixel+x*32, topYpixel+y*32);
		}
	}
}

void Game::Draw(sf::RenderWindow &target) {
	const int tileSize = 32;
	const int drawWidth = 40;
	const int drawHeight = 30;
	DrawTiles(target, (-data->center_x)%tileSize-tileSize, (-data->center_y)%tileSize-tileSize, drawWidth, drawHeight, data->mainworld, 
	(data->center_x)/tileSize-(1024/tileSize)/2-1, (data->center_y)/tileSize-(768/tileSize)/2-1);
	
	sort(data->placeables.begin(), data->placeables.end(), sort_placeable);
	
	for (const auto& placeable : data->placeables) {
		const MistItem *m = dynamic_cast<MistItem*>(placeable.get());
		if (m) {
			DrawMiscItem(target, data->sprites, m, data->time, data->center_x-1024/2, data->center_y-768/2);
		}
		const Human *h = dynamic_cast<Human*>(placeable.get());
		if (h) {
			DrawHumanEntity(target, data->sprites, h, data->time, data->center_x-1024/2, data->center_y-768/2);
		}
	}
}

static vector<pair<long long int, long long int> > getTouchingTiles(const Placeable* entity) {
	const long long int tileSize = 32;
	vector<pair<long long int, long long int> > res;
	vector<pair<long long int, long long int> > candidates;
	for (long long int i = floor((entity->X-entity->Radius) / tileSize); i < (entity->X+entity->Radius)/ tileSize ; i++ ) {
		for (long long int j = floor((entity->Y-entity->Radius) / tileSize); j < (entity->Y+entity->Radius)/ tileSize ; j++ ) {
			candidates.push_back(make_pair(i,j));
		}
	}
	res = candidates;
	return res;
}

static bool MoveEntity( Placeable* entity, const vector<shared_ptr<Placeable> > &collidables, World &world, const TileManager &tileManager, float destX, float destY ) {
	bool canMove = true;
	float sourceX = entity->X;
	float sourceY = entity->Y;
	entity->X = destX;
	entity->Y = destY;
	for ( const shared_ptr<Placeable>& item : collidables) {
		if (item.get() == entity ) {
			continue;
		}
		if (IsTouching(*entity, *item)) {
			canMove = false;
		}
	}
	if ( canMove) {
		auto tiles = getTouchingTiles(entity);
		for (auto tile : tiles) {
			if (tileManager.GetTile(world.GetTile(tile.first, tile.second)).blocking) {
				canMove = false;
			}
		}
	}
	if ( !canMove ) {
		entity->X = sourceX;
		entity->Y = sourceY;
	}
	return canMove;
}

static void MoveHumanEntity (Creature *entity, vector<shared_ptr<Placeable> > collidables, World &world, const TileManager &tileManager, float directionX, float directionY, float fDeltaTime) {
	float deltaX = directionX;
	float deltaY = directionY;
	if (deltaX == 0.0f && deltaY == 0.0f) {
		entity->moving = false;
		return;
	}
	entity->moving = true;
	if (deltaX*deltaX+deltaY*deltaY > 1.5f) {
		deltaX *= 0.7071067811865476f; //sqrt(0.5)
		deltaY *= 0.7071067811865476f; //sqrt(0.5)
	}
	if (deltaY > 0.0f) {
		entity->direction = 'S';
	}
	if (deltaY < 0.0f) {
		entity->direction = 'N';
	}
	if (deltaX < 0.0f) {
		entity->direction = 'W';
	}
	if (deltaX > 0.0f) {
		entity->direction = 'E';
	}
	float speed = 4.0f;
	MoveEntity (entity, collidables, world, tileManager, entity->X + deltaX*(fDeltaTime/speed), entity->Y + deltaY*(fDeltaTime/speed));
}

void Game::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	data->time += fDeltaTime;
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	if (input.IsPressed("DOWN")) {
		deltaY += 1.0f;
	}
	if (input.IsPressed("UP")) {
		deltaY -= 1.0f;
	}
	if (input.IsPressed("LEFT")) {
		deltaX -= 1.0f;
	}
	if (input.IsPressed("RIGHT")) {
		deltaX += 1.0f;
	}
	MoveHumanEntity(data->human.get(), data->placeables, data->mainworld, *(data->tileManager), deltaX, deltaY, fDeltaTime);
	data->center_x = round(data->human->X);
	data->center_y = round(data->human->Y);
	CheckCollision(data->placeables);
	//cout << round(data->human->X/32) << " " << round(data->human->Y/32) << endl;
}

void Game::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}