#pragma once

#include "furniture.hpp"

#include "../foods.hpp"

#define FOOD_Y_OFFSET	-20

class Stove : public Furniture {

public:

	Stove(int x, int y);
	~Stove();

	void render(int camX, int camY) override;
	void update() override;

	// Utility functions
	bool hasFood() const { return has_food; }
	void cookFood(Food food);

private:

	// The food that the stove is currently cooking
	bool has_food, on;
	Food current;
	Timer cook_timer;
	Texture * food_texture;

	static Texture * texture;
	static Texture * cook_texture;

};