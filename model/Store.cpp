/* 
 * File:   Store.cpp
 * Author: poul
 * 
 * Created on 29. december 2014, 17:38
 */

#include "Store.hpp"

using namespace std;

Store::Store() {
}

Store::~Store() {
}

void Store::Open(const std::string& filename) {
	string connection_string = "sqlite3:db="+filename;
	sql = std::unique_ptr<cppdb::session>(new cppdb::session(connection_string));
}