/* 
 * File:   StoreJson.cpp
 * Author: poul
 * 
 * Created on 12. januar 2015, 20:18
 */

#include "StoreJson.hpp"

using namespace std;

StoreJson::StoreJson() {
}

StoreJson::~StoreJson() {
}

void StoreJson::StoreWorld ( const std::string& name, const World& w) {
	Json::Value root;   // will contains the root value after parsing
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( file_content, root );
	if (!parsingSuccessful) {
		cerr << "Failed to store world" << endl;
		return;
	}
	
	Json::StyledWriter writer;
	file_content = writer.write( root );
}

void ReStoreWorld ( const std::string& name, World& w) {
	
}