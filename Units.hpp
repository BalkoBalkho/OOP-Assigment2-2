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
	bool render(POS position_) {
		//Unit sprite, there will be duplicate ids but this will always return false (will only be here for one frame)so it shoudlnt be an issue.
		Drawing::dl.add("u",[=]() {
			DrawTextureRec(Drawing::spritesheet.getSpritesheet(), Drawing::spritesheet.getSprite(lookup), position_, WHITE);
			return false;
		});
		
		
	}
};

	POS position; //position of the unit in the game world
	Sprite sprite;
	

	//there can only be one copy of Unit, copy constructer moves the unit and deletes the original.
	//idk 
	Unit(const Unit&) = delete;
	Unit& operator=(const Unit&) = delete;
	virtual ~Unit() = default; // Important for polymorphic destruction
};

class Monster : public Unit {
	uint16_t health;
	uint16_t speed;
	uint16_t morale;
	std::string_view name;
	std::string_view description;
	uint16_t dodge;
	bool corpse_sprite;
	uint16_t vision_day;
	uint16_t damage_amount;


	int getdifficulty() { //how dangerous this monster is.
		return (health / 10) + speed + morale / 10 + dodge + vision_day * 2 + damage_amount * 5;
	}
	

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


