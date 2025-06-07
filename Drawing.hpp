#pragma once
#include "raygui.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>

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