#include "chef.hpp"

#include "../foods.hpp"
#include "../util.hpp"
#include "../game.hpp"

#include "../furniture/stove.hpp"

// Definition of static variables
Texture * Chef::texture;
Texture * Chef::test;

Chef::Chef(int x, int y) :
	Staff(StaffType::CHEF, x, y),
	state(ChefState::IDLE)
{
	if (!texture) texture = new Texture("assets/chef.png");
	if (!test) test = new Texture("assets/test3.png");
}

Chef::~Chef() {

}

void Chef::update(Game& game) {
	switch (state) {
	case ChefState::IDLE: {
		// If the waiter is idle, find an event for him/her to do
		findChefEvent(game);
	} break;
	case ChefState::WALK_TO_COUNTER_FOR_ORDER: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			if (pathIndex >= currentPath.path.size() - 1) {
				state = ChefState::PAUSE1;
				findEmptyStovePath(game);
				pathIndex = 0;
			} else {
				moveTimer.reset(true);
			}
		}
	} break;
	case ChefState::PAUSE1: {
		// Just use the moveTimer lol
		if (moveTimer.getTicks() > PAUSE1_TIME) {
			state = ChefState::WALK_TO_STOVE;
			moveTimer.reset(true);
		}
	} break;
	case ChefState::WALK_TO_STOVE: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			if (pathIndex >= currentPath.path.size() - 1) {
				state = ChefState::IDLE;
				currentPath = Path();
				// Start cooking the food
				stove->cookFood(BasicUdon);
			} else {
				moveTimer.reset(true);
			}
		}
	} break;
	default: {
		// Do nothing here
	} break;
	}
}

void Chef::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Chef::renderMapDebug(int camX, int camY) {
	// RENDER WEIRD THINGS
	for (const Vec2& v : currentPath.path) {
		Vec2 coords = isometric(v);
		coords.y -= BASE_TILE_HEIGHT;
		test->render(coords.x - camX, coords.y - camY);
	}
}

bool Chef::findChefEvent(Game & game) {
	// If an order is in, go to it
	if (game.getEvent(EventType::ORDER_IN)) {
		handleOrderInEvent(game);
	}
	return false;
}

bool Chef::handleOrderInEvent(Game & game) {
	OrderInEvent * event = static_cast<OrderInEvent*>
		(game.getEvent(EventType::ORDER_IN));
	{	// Walk to the counter before taking the order
		bool adjacentToCounter = false;
		if (std::abs(event->counter->getX() - x) + std::abs(event->counter->getY() - y) == 1) {
			adjacentToCounter = true;
		}
		// If the chef isn't adjacent to THE counter, walk to it
		if (!adjacentToCounter) {
			Vec2 target = game.findEmptyNearSquare(Vec2(event->counter->getX(), event->counter->getY()), 1);
			currentPath = game.getPath(Vec2(x, y), target);
			if (currentPath.path.size() > 0) {
				state = ChefState::WALK_TO_COUNTER_FOR_ORDER;
				pathIndex = 0;
				moveTimer.reset(true);
				delete game.popEvent(EventType::ORDER_IN);
				return true;
			} else {
				return false;
			}
		}
	}
	// TODO: Notify the counter that the order has been taken
	return false;
}

void Chef::findEmptyStovePath(Game & game) {
	for (Furniture * f : game.getFurniture()) {
		if (f->getType() == FurnitureType::STOVE) {
			stove = dynamic_cast<Stove*>(f);
			// Check that the stove is empty
			Vec2 target = game.findEmptyNearSquare(Vec2(f->getX(), f->getY()), 1, true);
			currentPath = game.getPath(Vec2(x, y), target);
			return;
		}
	}
}
