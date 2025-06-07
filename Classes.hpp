#include <functional>
#include <string>
#include <stdexcept>
#include <memory>

struct POS {
	int x, y; //position of the object in the game world
	POS(int x, int y) : x(x), y(y) {}
	bool operator==(const POS& other) const {
		return x == other.x && y == other.y;
	}
	bool operator!=(const POS& other) const {
		return !(*this == other);
	}
};

class Unit;
class GameState {
public:
	std::list<Unit> units; //reference these units by pointers.

};


class Building//dungeon
{	

};
class Tile {
	Unit* unit = nullptr; //pointer to the unit on this tile, if any

public:
	enum class TileType {
		Empty, //no tile
		Floor, //floor tile
		Wall, //wall tile
		Door, //door tile
		StairsUp, //stairs up tile
		StairsDown, //stairs down tile
	};
private:     TileType type;
public:
	Tile() : type(TileType::Empty), unit(nullptr) {} // Default constructor
	Tile(TileType t) : type(t), unit(nullptr) {}
};
class Room {
	
	std::vector<std::vector<Tile>> tiles; //2D array of tiles, each tile is a pointer to a tile object.
	
	//class BSP {
	//	struct Node {
	//		int x, y; //position of the node
	//		int width, height; //size of the node
	//		Node* left; //left child
	//		Node* right; //right child
	//	};
	//	Node* root; //root node of the BSP tree
	//	BSP(int length, int height) : root(nullptr) {
	//		//initialize the BSP tree with the given dimensions
	//		root = new Node{ 0, 0, length, height, nullptr, nullptr };
	//		// randomly split the tree into smaller nodes to get procedural rooms.
	//		std::rand();
	//	}
	//};
	// create walls and rooms in the room (with gaps for doors)
	void partition(int width, int length) {
		

	}

	Room() {

	}
};

//abstract

class Unit {
	int health; //health of the unit
	POS position; //position of the unit in the game world

	//there can only be one copy of Unit, copy constructer moves the unit and deletes the original.
	
};

class Meat {


};

class ItemOnGround : public Unit {

};

class Item {
	std::string name; //name of the item
	std::string description; //description of the item

	virtual void operator()() = 0;
	POS position; //position of the item in the game world
	

};
class Player :public Unit{
	std::vector<Item> inventory;
};


