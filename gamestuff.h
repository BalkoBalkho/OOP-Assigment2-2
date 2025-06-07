#pragma once
#include <list>
#include "raylib.h"
#include <queue>

struct POS {
	int x, y; //position of the object in the game world
	POS(int x, int y) : x(x), y(y) {}
	bool operator==(const POS& other) const {
		return x == other.x && y == other.y;
	}
	bool operator!=(const POS& other) const {
		return !(*this == other);
	}
	   //explicit idk 
	   operator Vector2() const {
        return {(float)x, (float)y};
    }
};

class Unit;
class GameState {
	//proggresive difficulty, it goes up and down based on the player's performance and in a sinusodial pattern
	std::queue<uint16_t> difficulty;
public:
	std::list<Unit> units; //reference these units by pointers.
	uint16_t getDifficulty() const {
		if (difficulty.empty()) return 0; //if the queue is empty, return 0
		return difficulty.front(); //return the front of the queue
	}

};