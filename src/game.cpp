#include "game.hpp"

#include <ctime>

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
	srand(static_cast<unsigned int>(time(NULL)));
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
	staff.push_back(new Waiter(3, 3));
	staff.push_back(new Chef(4, 2));

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
		c->update(*this);
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
	{	// Render any debugging things that should go on the bottom
		for (Staff * s : staff) s->renderMapDebug(camX, camY);
		for (Customer * c : customers) c->renderMapDebug(camX, camY);
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

Event * Game::getEvent(EventType type) {
	if (events[static_cast<int>(type)]) return events[static_cast<int>(type)]->event;
	return nullptr;
}

Event * Game::popEvent(EventType type) {
	if (events[static_cast<int>(type)]) {
		EventWrapper * head = events[static_cast<int>(type)];
		events[static_cast<int>(type)] = head->next;
		Event * event = head->event;
		delete head;
		return event;
	}
	return nullptr;
}

Event * Game::pushEvent(Event * event) {
	EventWrapper * wrapper = new EventWrapper{event, nullptr};
	// Get the current last item in the linked list to append to
	EventWrapper * head = events[static_cast<int>(event->type)];
	while (head && head->next != nullptr) head = head->next;
	// Append the new event
	if (head) head->next = wrapper;
	else events[static_cast<int>(event->type)] = wrapper;
	return event;
}

EventWrapper * Game::getEventHead(EventType type) {
	return events[static_cast<int>(type)];
}

Path Game::getPath(Vec2 start, Vec2 end, bool chair_off) {
	Path target;
	if (findPath(start, end, target, chair_off)) {
		return target;
	}
	// If the path doesn't exist, return an empty path
	return Path();
}

bool Game::positionValid(Vec2 pos) const {
	if (pos.x < 0 || pos.y < 0) return false;
	if (pos.x >= map_size || pos.y >= map_size) return false;
	for (const Furniture * f : furniture) {
		if (f->getX() == pos.x && f->getY() == pos.y) return false;
	}
	for (const Staff * s : staff) {
		if (s->getX() == pos.x && s->getY() == pos.y) return false;
	}
	for (const Customer * c : customers) {
		if (c->getX() == pos.x && c->getY() == pos.y) return false;
	}
	return true;
}

// threshold represents the max number of tries before giving up
Vec2 Game::findEmptyNearSquare(Vec2 target, int range, bool square, int threshold) {
	while (threshold > 0) {
		// Use a square range for now
		int x_offset = rand() % (2 * range + 1) - range;
		int y_offset = rand() % (2 * range + 1) - range;
		if (x_offset == 0 && y_offset == 0) {
			threshold++;
			continue;
		}
		if (!square && (std::abs(x_offset) + std::abs(y_offset) > range)) {
			threshold++;
			continue;
		}
		Vec2 result = target;
		result.x += x_offset;
		result.y += y_offset;
		if (positionValid(result)) return result;
	}
	// Return an invalid result if the function failed
	return Vec2(-1, -1);
}

void Game::handleCustomerSpawns() {
	// For now, set the logic to spawn a customer if there are none
	if (customers.size() < MAX_CUSTOMERS && positionValid(Vec2(entrance_x, entrance_y))) {
		Customer * newCustomer = new Customer(entrance_x, entrance_y);
		customers.push_back(newCustomer);
		// Add a customer arrived event
		CustomerArriveEvent * event = new CustomerArriveEvent;
		event->type =		EventType::CUSTOMER_ARRIVE;
		event->customer =	newCustomer;
		pushEvent(event);
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

/*
 * Pathfinding function that modifies @target to be the path from @start to @and
 *	- returns false if no path was found
 */
bool Game::findPath(Vec2 start, Vec2 end, Path & target, bool chair_off) {

	// if the start and end is the same tile just return an empty path
	if (start.x == end.x && start.y == end.y) {
		target = Path();
		return true;
	} else {
		// copy map collision data into a temp map
		std::vector<int> collision_map(map_size * map_size, 1);
		// add the entity data into the collision data
		for (Furniture * f : furniture) {
			if (chair_off && f->getType() == FurnitureType::CHAIR) continue;
			if (end.x == f->getX() && end.y == f->getY()) return false;
			collision_map[MAP_INDEX(f->getX(), f->getY())] = 0;
		}
		for (Staff * s : staff) {
			if (end.x == s->getX() && end.y == s->getY()) return false;
			collision_map[MAP_INDEX(s->getX(), s->getY())] = 0;
		}
		for (Customer * c : customers) {
			if (end.y == c->getX() && end.y == c->getY()) return false;
			collision_map[MAP_INDEX(c->getX(), c->getY())] = 0;
		}
		// setup open and closed list of tiles
		std::vector<Path> open;
		std::vector<Vec2> closed;
		// add initial tile to the open set
		int starting_score = std::abs(start.x - end.x) + std::abs(start.y - end.y);
		open.push_back({ std::vector<Vec2>({ Vec2(start.x, start.y) }), 0, starting_score });
		// while there are paths in the open list, look for the optimal path
		while (open.size() > 0) {
			// find the current path in open with the smallest score
			Path shortest = open[0];
			for (const Path& p : open) {
				int score = p.cost + (std::abs(p.path[p.path.size() - 1].x - end.x)) +
					(std::abs(p.path[p.path.size() - 1].y - end.y));
				if (score < shortest.score) {
					shortest = p;
				}
			}
			// then remove it from open and add it to closed
			closed.push_back(shortest.path[shortest.path.size() - 1]);
			open.erase(std::find(open.begin(), open.end(), shortest));
			// then compute adjacent squares
			Vec2 cur_pos = shortest.path[shortest.path.size() - 1];
			// std::cout << cur_pos.x << ", " << cur_pos.y << " -> ";
			// std::cout << MAP_INDEX(cur_pos.x, cur_pos.y - 1) << std::endl;
			if (cur_pos.y > 0 && collision_map[MAP_INDEX(cur_pos.x, cur_pos.y - 1)] != 0) {	// above
				Vec2 new_pos = cur_pos;
				new_pos.y--;
				if (new_pos.x >= 0 && new_pos.x <= map_size && new_pos.y >= 0 && new_pos.y <= map_size) {
					if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
				}
			}
			if (cur_pos.x < map_size - 1 && collision_map[MAP_INDEX(cur_pos.x + 1, cur_pos.y)] != 0) {	// right
				Vec2 new_pos = cur_pos;
				new_pos.x++;
				if (new_pos.x >= 0 && new_pos.x <= map_size && new_pos.y >= 0 && new_pos.y <= map_size) {
					if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
				}
			}
			if (cur_pos.y < map_size - 1 && collision_map[MAP_INDEX(cur_pos.x, cur_pos.y + 1)] != 0) {	// below
				Vec2 new_pos = cur_pos;
				new_pos.y++;
				if (new_pos.x >= 0 && new_pos.x <= map_size && new_pos.y >= 0 && new_pos.y <= map_size) {
					if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
				}
			}
			if (cur_pos.x > 0 && collision_map[MAP_INDEX(cur_pos.x - 1, cur_pos.y)] != 0) {	// left
				Vec2 new_pos = cur_pos;
				new_pos.x--;
				if (new_pos.x >= 0 && new_pos.x <= map_size && new_pos.y >= 0 && new_pos.y <= map_size) {
					if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
				}
			}
		}
	}
	return false;

}

bool Game::pathfindHelper(Vec2 end, std::vector<Path>& open, std::vector<Vec2>& closed, Vec2 new_pos, const Path& shortest, Path& target) {
	// check to see if we have our desired path already
	if (new_pos.x == end.x && new_pos.y == end.y) {
		std::vector<Vec2> new_path = shortest.path;
		new_path.push_back(new_pos);
		target = { new_path, shortest.cost + 1 };
		return true;
	}
	// check to make sure new position is not already in closed
	for (const Vec2& v : closed) {
		if (new_pos == v) {
			return false;
		}
	}
	bool found_open = false;
	// check to see if this path is in the open list
	for (Path & p : open) {
		if (new_pos == p.path[p.path.size() - 1]) {
			// compute the score and compare if we found the same one
			int old_cost = p.score;
			int new_cost = shortest.score + 1;
			if (new_cost < old_cost) {
				std::vector<Vec2> new_path = shortest.path;
				new_path.push_back(new_pos);
				int new_score = std::abs(new_pos.x - end.x) + std::abs(new_pos.y - end.y) + shortest.cost + 1;
				p = Path{ new_path, shortest.cost + 1, new_cost };
			}
			found_open = true;
		}
	}
	// finally, add the path directly if we didn't find a duplicate
	if (!found_open) {
		std::vector<Vec2> new_path = shortest.path;
		new_path.push_back(new_pos);
		int new_score = std::abs(new_pos.x - end.x) + std::abs(new_pos.y - end.y) + shortest.cost + 1;
		open.push_back({ new_path, shortest.cost + 1, new_score });
	}
	return false;
}

void Game::renderDebug() {
	// render the debug title
	DebugRenderer debug(*this, 1000, 50);
	debug.renderDebugString("DEBUG");
	// cursor tile x and y messages
	debug.renderDebugString(std::string("cursor x: ") + std::to_string(cursor_tile_x));
	debug.renderDebugString(std::string("cursor y: ") + std::to_string(cursor_tile_y));
}