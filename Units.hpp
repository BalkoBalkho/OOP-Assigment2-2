#pragma once  
#include "gamestuff.h"  
#include <string>  
#include <vector>  
#include "Drawing.hpp"  
//abstract  


class Unit {  
	
	public:  
	class Sprite {  
		uint16_t lookup; //index in tiles.png  
		bool render(POS position_) {  
			//Unit sprite, there will be duplicate ids but this will always return false (will only be here for one frame)so it shoudlnt be an issue.  
			Drawing::dl.add("u", [=]() {  
				DrawTextureRec(Drawing::ss.getSpritesheet(), Drawing::ss.getSprite(lookup), position_, WHITE);  
				return false;  
			});  
		}  
	public:
		Sprite(uint16_t lookup_index) : lookup(lookup_index) {}  
	};  

protected:  
	POS position; //position of the unit in the game world  

	Sprite sprite;  
public:  
	// Default constructor explicitly defined to resolve the issue  
	Unit() : position(0, 0), sprite(0) {}  

	// Virtual destructor for polymorphic destruction  
	virtual ~Unit() = default;  

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

public:  
	int getdifficulty() { //how dangerous this monster is.  
		return (health / 10) + speed + morale / 10 + dodge + vision_day * 2 + damage_amount * 5;  
	}  

	void move(Room rom,POS playerpos) {
		

	}
	// Monster constructor initializing all member variables  
	/*Monster(POS pos, int hp, int speed, int morale, int damage_amount, std::string_view name, std::string_view description, uint16_t dodge, bool corpse_sprite, uint16_t vision_day)  
		: health(hp),  
    speed(speed),  
    morale(morale),  
    name(name),  
    description(description),  
    dodge(dodge),  
    corpse_sprite(corpse_sprite),  
    vision_day(vision_day),  
    damage_amount(damage_amount),  
    Unit() {  
		this->position = pos;  
	}  */


	Monster(POS pos, uint16_t sprite_lookup_index, int hp, int speed, int morale, int damage_amount, std::string_view name, std::string_view id)
		: health(hp),
		speed(speed),
		morale(morale),
		name(name),
		description(id), // Assuming 'id' is used as a description placeholder  
		dodge(0),        // Default value for dodge  
		corpse_sprite(false), // Default value for corpse_sprite  
		vision_day(0),   // Default value for vision_day  
		damage_amount(damage_amount),
		Unit() {
		this->position = pos;
		this->sprite = Sprite(sprite_lookup_index); // Initialize sprite with lookup index  
	}

	virtual ~Monster() override = default;  
};  

class ItemOnGround : public Unit {  

};  

class Item {  
	std::string name; //name of the item  
	std::string description; //description of the item  

	virtual void operator()() = 0;  
	POS position; //position of the item in the game world  
};  

class Player : private Unit {  
	std::vector<Item> inventory;  
	
};
