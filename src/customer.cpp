#include "customer.hpp"

#include "util.hpp"

Texture * Customer::texture;

Customer::Customer(int x, int y) : x(x), y(y) {
	if (!texture) texture = new Texture("assets/customer.png");
}

Customer::~Customer() {

}

void Customer::update() {

}

void Customer::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}
