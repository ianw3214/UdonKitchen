#include "table.hpp"

#include "../util.hpp"

// Definition of static variables
Texture * Table::texture;

Table::Table(int x, int y) :
	Furniture(FurnitureType::TABLE, x, y)
{
	if (!texture) texture = new Texture("assets/table.png");
}

Table::~Table() {

}

void Table::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Table::update() {

}
