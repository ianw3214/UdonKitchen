#include "waiter.hpp"

#include "../util.hpp"
#include "../game.hpp"

// Definition of static variables
Texture * Waiter::texture;

Waiter::Waiter(int x, int y) :
	Staff(StaffType::WAITER, x, y)
{
	if (!texture) texture = new Texture("assets/waiter.png");
}

Waiter::~Waiter() {

}

void Waiter::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Waiter::update(Game& game) {
	
}
