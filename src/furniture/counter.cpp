#include "counter.hpp"

#include "../util.hpp"

// Definition of static variables
Texture * Counter::texture;

Counter::Counter(int x, int y) :
	Furniture(FurnitureType::COUNTER, x, y)
{
	if (!texture) texture = new Texture("assets/counter.png");
}

Counter::~Counter() {

}

void Counter::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Counter::update() {

}
