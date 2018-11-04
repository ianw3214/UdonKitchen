#include "game.hpp"

#include "util.hpp"

#include "furniture/furniture.hpp"
#include "furniture/stove.hpp"
#include "furniture/table.hpp"
#include "furniture/chair.hpp"
#include "furniture/counter.hpp"
#include "furniture/stand.hpp"
#include "staff/staff.hpp"
#include "staff/chef.hpp"
#include "staff/waiter.hpp"
#include "customer.hpp"

Game::Game() :
	map_size(DEFAULT_MAP_SIZE),
	entrance_x(DEFAULT_ENTRANCE_X),
	entrance_y(DEFAULT_ENTRANCE_Y)
{
	createFont("debug", "assets/fonts/Munro.ttf", 16);

	// Initialize textures
	tile = new Texture("assets/tile.png");
	hover = new Texture("assets/hover.png");

	/*
	furniture.push_back(new Stove(3, 3));
	furniture.push_back(new Table(4, 4));
	furniture.push_back(new Stand(5, 5));
	furniture.push_back(new Counter(6, 6));
	furniture.push_back(new Chair(7, 7));

	staff.push_back(new Chef(0, 0));
	staff.push_back(new Waiter(1, 1));

	customers.push_back(new Customer(2, 2));
	*/

	// Temporary code to add default furniture
	furniture.push_back(new Stand(0, 1));
	furniture.push_back(new Stove(7, 0));
	furniture.push_back(new Stove(7, 1));
	furniture.push_back(new Counter(7, 3));
	furniture.push_back(new Counter(6, 3));
	furniture.push_back(new Counter(5, 3));
	furniture.push_back(new Table(2, 7));
	furniture.push_back(new Chair(1, 7));
	furniture.push_back(new Chair(3, 7));
	furniture.push_back(new Table(2, 5));
	furniture.push_back(new Chair(1, 5));
	furniture.push_back(new Chair(3, 5));
	furniture.push_back(new Table(2, 4));
	furniture.push_back(new Chair(1, 4));
	furniture.push_back(new Chair(3, 4));
	furniture.push_back(new Table(5, 7));
	furniture.push_back(new Chair(4, 7));
	furniture.push_back(new Chair(6, 7));

	// Temporary code to add default employees
	staff.push_back(new Waiter(0, 0));

	// Center the camera by default
	Vec2 center = isometric(Vec2(map_size / 2, map_size/ 2));
	camX = - getWindowWidth() / 2 + center.x;
	camY = - getWindowHeight() / 2 + center.y;
}

Game::~Game() {

}

void Game::init() {

}

void Game::cleanup() {

}

void Game::pause() {

}

void Game::resume() {

}

void Game::update() {
	// Update the cursor tile
	getCursorTile();

	// Update events
	if (keyPressed(SDL_SCANCODE_ESCAPE)) {
		managerRef->quit();
	}

	// Call various game logic handlers
	handleCustomerSpawns();

	// Update the actual entities
	for (Furniture * f : furniture) {
		f->update();
	}
	for (Staff * s : staff) {
		s->update(*this);
	}
	for (Customer * c : customers) {
		c->update();
	}
}

void Game::render() {
	// render the tile map
	for (int y = 0; y < map_size; y++) {
		for (int x = 0; x < map_size; x++) {
			Vec2 coords = isometric(Vec2(x, y));
			coords.y -= BASE_TILE_HEIGHT;
			tile->render(coords.x - camX, coords.y - camY);
			
		}
	}
	{	// Render the hover sprite if the cursor is on the tile
		Vec2 coords = isometric(Vec2(cursor_tile_x, cursor_tile_y));
		coords.y -= BASE_TILE_HEIGHT;
		hover->render(coords.x - camX, coords.y - camY);
	}

	for (int y = map_size - 1; y >= 0; y--) {
		for (int x = 0; x < map_size; x++) {
			// Render furniture
			for (Furniture * f : furniture) {
				if (f->getX() == x && f->getY() == y) f->render(camX, camY);
			}
			// Render staff
			for (Staff * s : staff) {
				if (s->getX() == x && s->getY() == y) s->render(camX, camY);
			}
			// Render customers
			for (Customer * c : customers) {
				if (c->getX() == x && c->getY() == y) c->render(camX, camY);
			}
		}
	}

	renderDebug();
}

void Game::handleCustomerSpawns() {
	// For now, set the logic to spawn a customer if there are none
	if (customers.size() == 0) {
		customers.push_back(new Customer(entrance_x, entrance_y));
	}
}

/*
* Helper function to calculate the tile x and y position of the cursor from screen position
*/
void Game::getCursorTile() {
	Vec2 cart = cartesian(Vec2(getMouseX() + camX, getMouseY() + camY));
	cursor_tile_x = cart.x / BASE_TILE_WIDTH;
	cursor_tile_y = cart.y / BASE_TILE_WIDTH;
	// If the cartesian coordinates is negative, decrement it by one
	if (cart.x < 0) cursor_tile_x--;
	if (cart.y < 0) cursor_tile_y--;
}

void Game::renderDebug() {
	// render the debug title
	DebugRenderer debug(*this, 1000, 50);
	debug.renderDebugString("DEBUG");
	// cursor tile x and y messages
	debug.renderDebugString(std::string("cursor x: ") + std::to_string(cursor_tile_x));
	debug.renderDebugString(std::string("cursor y: ") + std::to_string(cursor_tile_y));
}