#include "chair.hpp"

#include "../util.hpp"

// Definition of static variables
Texture * Chair::texture;

Chair::Chair(int x, int y) :
	Furniture(FurnitureType::CHAIR, x, y)
{
	if (!texture) texture = new Texture("assets/chair.png");
}

Chair::~Chair() {

}

void Chair::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Chair::update() {

}
