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
	enum class TileType {
		Empty, //no tile
		Floor, //floor tile
		Wall, //wall tile
		Door, //door tile
		StairsUp, //stairs up tile
		StairsDown, //stairs down tile
	};

	Unit* unit = nullptr; //pointer to the unit on this tile, if any

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


//used for UI
struct DrawFunctor {
	std::string id; //name of the functor, used for debugging
	std::function<bool()> func; //the function to call
	DrawFunctor(const std::string& name, std::function<bool()> func) : id(name), func(func) {}
	// if functor returns false, it will be removed from the list.
	bool operator()() { return func(); } //call the function
};
class DrawList {
	//provide a functor that will execute raylib draw calls.
	//functors are perfect for this, as they can capture state like how UI has state.
	std::vector<std::unique_ptr<DrawFunctor>> drawFunctions;
public:
	DrawFunctor getFromId(const std::string& id) const {
		for (const auto& func : drawFunctions) {
			if (func.id == id) {
				return func;
			}
		}
		throw std::runtime_error("DrawFunctor with id " + id + " not found.");
	}
	void add(const DrawFunctor& functor) {
		drawFunctions.push_back(functor);
	}
	void remove(const std::string& id) {
		drawFunctions.erase(std::remove_if(drawFunctions.begin(), drawFunctions.end(),
			[&id](const DrawFunctor& f) { return f.id == id; }), drawFunctions.end());
	}
	void draw() {
		for (auto& func : drawFunctions) {
			if (!func()) { //if the function returns false, remove it
				remove(func.id);
			}
		}
	}
	

	
	
	

};