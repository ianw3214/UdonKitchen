#include "stove.hpp"

#include "../util.hpp"

// Definition of static variables
Texture * Stove::texture;
Texture * Stove::cook_texture;

Stove::Stove(int x, int y) :
	Furniture(FurnitureType::STOVE, x, y),
	has_food(false),
	on(false),
	current({"", 0, 0})
{
	if (!texture) texture = new Texture("assets/stove.png");
	if (!cook_texture) cook_texture = new Texture("assets/stove_on.png");
}

Stove::~Stove() {

}

void Stove::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	if (on) {
		cook_texture->render(coords.x - camX, coords.y - camY);
	} else {
		texture->render(coords.x - camX, coords.y - camY);
	}
	if (has_food) {
		food_texture->render(coords.x - camX, coords.y - camY + FOOD_Y_OFFSET);
	}
}

void Stove::update() {
	if (on) {
		if (cook_timer.getTicks() > current.time) {
			on = false;
			// Send an event telling the game that the food is done
		}
	}
}

void Stove::cookFood(Food food) {
	has_food = true;
	on = true;
	current = food;
	cook_timer.reset(true);
	if (food_texture) delete food_texture;
	food_texture = new Texture(current.file_path);
}
