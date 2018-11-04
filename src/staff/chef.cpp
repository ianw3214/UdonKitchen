#include "chef.hpp"

#include "../util.hpp"
#include "../game.hpp"

// Definition of static variables
Texture * Chef::texture;

Chef::Chef(int x, int y) :
	Staff(StaffType::CHEF, x, y)
{
	if (!texture) texture = new Texture("assets/chef.png");
}

Chef::~Chef() {

}

void Chef::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Chef::update(Game& game) {

}
