/* 
 * File:   StoreJson.hpp
 * Author: poul
 *
 * Created on 12. januar 2015, 20:18
 */

#ifndef STOREJSON_HPP
#define	STOREJSON_HPP

#include "World.hpp"

class StoreJson {
public:
	StoreJson();
	StoreJson(const StoreJson& orig) = delete;
	virtual ~StoreJson();
	
	void StoreWorld ( const std::string& name, const World& w);
	void ReStoreWorld ( const std::string& name, World& w);
private:
	std::string file_content;
};

#endif	/* STOREJSON_HPP */

