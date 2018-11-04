#include "stand.hpp"

#include "../util.hpp"

// Definition of static variables
Texture * Stand::texture;

Stand::Stand(int x, int y) :
	Furniture(FurnitureType::STAND, x, y)
{
	if (!texture) texture = new Texture("assets/stand.png");
}

Stand::~Stand() {

}

void Stand::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Stand::update() {

}
