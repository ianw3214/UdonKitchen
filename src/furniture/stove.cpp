#include "stove.hpp"

#include "../util.hpp"

// Definition of static variables
Texture * Stove::texture;

Stove::Stove(int x, int y) :
	Furniture(FurnitureType::STOVE, x, y)
{
	if (!texture) texture = new Texture("assets/stove.png");
}

Stove::~Stove() {

}

void Stove::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Stove::update() {

}
