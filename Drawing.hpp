#pragma once
#include "raygui.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>

class Spritesheet
{
	static Texture2D spritesheet;
	const int SPRITE_WIDTH = 32;
	const int SPRITE_HEIGHT = 32;
	const int SPRITESHEET_WIDTH = 512;
	const int SPRITESHEET_HEIGHT = 18208; // tall, has a bunch of stuff that we dont care
	const int SPRITES_PER_ROW = SPRITESHEET_WIDTH / SPRITE_WIDTH;
	const int maxSpriteIndex = (SPRITESHEET_WIDTH * SPRITESHEET_HEIGHT) / (SPRITE_WIDTH * SPRITE_HEIGHT) - 1;

	public:
	Spritesheet()
	{
		spritesheet = LoadTexture("./data/tiles.png");
		if (spritesheet.id == 0)
		{
			throw std::runtime_error("Failed to load spritesheet: ./data/tiles.png");
		}
	}

	static const Texture2D& getSpritesheet() {
		return spritesheet;
	}
	Rectangle getSprite(uint32_t index)
	{
		if (index > maxSpriteIndex)
			throw std::out_of_range("Sprite index out of bounds");

		int col1 = index % SPRITES_PER_ROW;
		int row1 = index / SPRITES_PER_ROW;
		return {
			(float)col1 * SPRITE_WIDTH,
			(float)row1 * SPRITE_HEIGHT,
			(float)SPRITE_WIDTH,
			(float)SPRITE_HEIGHT};
	}
	~Spritesheet()
	{
		if (spritesheet.id != 0) // Check if the texture is loaded
		{
			UnloadTexture(spritesheet); // Unload the texture to free resources
			spritesheet.id = 0; // Reset the ID to indicate it's unloaded
		}
	}
};


//used for UI
	// if functor returns false, it will be removed from the list.

struct DrawFunctor {
	std::string id; //name of the functor, used for debugging
	DrawFunctor(const std::string& name) : id(name) {}
	virtual bool operator()() = 0;
	virtual ~DrawFunctor() = default; //virtual destructor for polymorphic use
};

struct LambdaDrawFunctor : public DrawFunctor {
	std::function<bool()> func;

	LambdaDrawFunctor(const std::string& name, std::function<bool()> f)
		: DrawFunctor(name), func(f) {
	}

	bool operator()() override {
		return func();
	}
};

class DrawList {
	//provide a functor that will execute raylib draw calls.
	//functors are perfect for this, as they can capture state like how UI has state.
	//pointer because polymorphism
	std::vector<std::unique_ptr<DrawFunctor>> drawFunctions;
public:
	//remove if there, if its empty do nothing
	void remove(const std::string& id) {
		for (auto it = drawFunctions.begin(); it != drawFunctions.end(); ++it) {
			if ((*it)->id == id) {
				drawFunctions.erase(it);
				return; // Assuming IDs are unique, stop after finding one
			}
		}
	
	}


	//temporary reference, you cannot safely keep it
	DrawFunctor* getFromId(const std::string& id) const {
		for (const auto& func : drawFunctions) {
			if (func->id == id) {
				return func.get();
			}
		}
		throw std::runtime_error("DrawFunctor with id " + id + " not found.");
	}

	// Overload 1: For adding a generic lambda/std::function
	// Creates a new LambdaDrawFunctor internally
	void add(const std::string& id, std::function<bool()> func) {
		drawFunctions.push_back(std::make_unique<LambdaDrawFunctor>(id, func));
	}

	void add(std::unique_ptr<DrawFunctor> functor_ptr) {
		if (!functor_ptr) {
			throw std::invalid_argument("Cannot add a null unique_ptr to DrawFunctor.");
		}
		drawFunctions.push_back(std::move(functor_ptr)); // Use std::move to transfer ownership
	}

	void draw() {
		BeginDrawing();
		for (auto it = drawFunctions.begin(); it != drawFunctions.end(); /* no increment here */) {
			if (!(*it)->operator()()) { // If the function returns false
				// Erase the current element. erase() returns an iterator to the next element.
				it = drawFunctions.erase(it);
			}
			else {
				// Only increment the iterator if no element was erased
				++it;
			}
		}
		EndDrawing();
	}
};

namespace Drawing {

	DrawList dl;
	Spritesheet spritesheet;

	
}