/* 
 * File:   TileManager.cpp
 * Author: poul
 * 
 * Created on 18. december 2014, 19:46
 */


#include "TileManager.hpp"
#include "../sago/SagoMisc.hpp"
#include <jsoncpp/json/json.h>

using namespace std;

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

static unsigned int GetNextId() {
	static unsigned int id = 1;  // 0 is reserved for void
	return id++;
}

void TileReadFile(TileManager &tileManager, const std::string &filename) {
	string fullfile = "tiles/"+filename;
	string content = sago::GetFileContent(fullfile.c_str());
	Json::Value root;   // will contains the root value after parsing
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( content, root );
	if ( !parsingSuccessful ) {
		cerr << "Failed to parse: " << fullfile << endl
				<< reader.getFormattedErrorMessages() << endl;
		return;
	}
	for (Json::Value::iterator it = root.begin(); it != root.end() ; ++it) {
		Tile t;
		t.internalId = GetNextId();
		t.name = it.memberName();
		Json::Value value = (*it);
		t.sprite = value.get("sprite","fallback").asString();
		t.blocking = value.get("blocking",0).asInt();
		tileManager.AddTile(t);
		cout << "Added " << t.name << " with id: " << t.internalId << endl;
	}
}

std::map<int, int> TileManager::getMapperFrom( const TileManager& source) {
	std::map<int, int> ret;
	for (const auto& item : source.idMap) {
		ret[item.second] = GetId(item.first);
	}
	return ret;
}