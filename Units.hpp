#pragma once
#include "gamestuff.h"
#include <string>
#include <vector>
#include "Drawing.hpp"


//abstract
class Unit {

	class Sprite {
	uint16_t lookup; //index in tiles.png
	public:
	Sprite(uint16_t lookup_index) : lookup(lookup_index){
	};
	bool render() {
		//Unit sprite, there will be duplicate ids but this will always return false (will only be here for one frame)so it shoudlnt be an issue.
		Drawing::dl.add("u",[=]() {
			DrawTextureRec(Drawing::spritesheet.getSpritesheet(), Drawing::(lookup), ()position, TRANS);
			return false;
		});
		
		
	}
};

	int health; //health of the unit
	POS position; //position of the unit in the game world
	Sprite sprite;

	//there can only be one copy of Unit, copy constructer moves the unit and deletes the original.

};



class ItemOnGround : public Unit {

};

class Item {
	std::string name; //name of the item
	std::string description; //description of the item

	virtual void operator()() = 0;
	POS position; //position of the item in the game world


};
class Player :public Unit {
	std::vector<Item> inventory;
};


