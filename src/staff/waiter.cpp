#include "waiter.hpp"

#include <algorithm>

#include "../game.hpp"
#include "../customer.hpp"

// Definition of static variables
Texture * Waiter::texture;
Texture * Waiter::test;

Waiter::Waiter(int x, int y) :
	Staff(StaffType::WAITER, x, y),
	state(WaiterState::IDLE),
	currentPath(),
	pathIndex(0)
{
	if (!texture) texture = new Texture("assets/waiter.png");
	if (!test) test = new Texture("assets/test.png");
}

Waiter::~Waiter() {

}

void Waiter::update(Game& game) {
	switch (state) {
	case WaiterState::IDLE: {
		// If the waiter is idle, find an event for him/her to do
		findWaiterEvent(game);
	} break;
	case WaiterState::WALK_TO_STAND: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			if (pathIndex >= currentPath.path.size() - 1) {
				state = WaiterState::IDLE;
				currentPath = Path();
			} else {
				moveTimer.reset(true);
			}
		}
	} break;
	case WaiterState::ESCORT_CUSTOMER: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			moveTimer.reset(true);
			// Wait for the customer once the waiter has reached the table
			if (pathIndex >= currentPath.path.size() - 1) {
				state = WaiterState::WAIT_FOR_CUSTOMER;
				currentPath = Path();
				// Send an event to the game
				game.pushEvent(nextEvent);
				nextEvent = nullptr;
			}
		}
	} break;
	case WaiterState::WALK_TO_CUSTOMER: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			moveTimer.reset(true);
			if (pathIndex >= currentPath.path.size() - 1) {
				state = WaiterState::IDLE;
				currentPath = Path();
			}
		}
	} break;
	case WaiterState::WAIT_FOR_CUSTOMER: {
		// If the customer is thinking, then he/she has reached their seat
		if (currentCustomer->getState() == CustomerState::THINKING) {
			state = WaiterState::IDLE;
			currentCustomer = nullptr;
		}
	} break;
	case WaiterState::DELIVER_ORDER: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			moveTimer.reset(true);
			if (pathIndex >= currentPath.path.size() - 1) {
				state = WaiterState::IDLE;
				currentPath = Path();
				// Send an event to the game
				game.pushEvent(nextEvent);
				nextEvent = nullptr;
			}
		}
	} break;
	default: {
		// Do nothing by default
	} break;
	}
}

void Waiter::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Waiter::renderMapDebug(int camX, int camY) {
	// RENDER WEIRD THINGS
	for (const Vec2& v : currentPath.path) {
		Vec2 coords = isometric(v);
		coords.y -= BASE_TILE_HEIGHT;
		test->render(coords.x - camX, coords.y - camY);
	}
}

bool Waiter::findWaiterEvent(Game & game) {
	// Find when a customer has arrived and service the customer
	if (game.getEvent(EventType::CUSTOMER_ARRIVE)) {
		if (handleCustomerArriveEvent(game)) return true;
	}
	// Find when a customer wants to order and take the order
	if (game.getEvent(EventType::CUSTOMER_ORDER)) {
		if (handleCustomerOrderEvent(game)) return true;
	}
	return false;
}

bool Waiter::handleCustomerArriveEvent(Game & game) {

	{	// Walk to the stand before waiting the customer
		bool adjacentToStand = false;
		Vec2 standPos;
		for (Furniture * f : game.getFurniture()) {
			if (f->getType() == FurnitureType::STAND) {
				if (std::abs(f->getX() - x) + std::abs(f->getY() - y) == 1) {
					adjacentToStand = true;
				}
				standPos = Vec2(f->getX(), f->getY());
			}
		}
		// If the waiter isn't adjacent to a stand, walk to one
		if (!adjacentToStand) {
			// TODO: actually find a path that's adjacent to the stand
			Vec2 target = game.findEmptyNearSquare(standPos, 1);
			currentPath = game.getPath(Vec2(x, y), target);
			if (currentPath.path.size() > 0) {
				state = WaiterState::WALK_TO_STAND;
				pathIndex = 0;
				moveTimer.reset(true);
				return true;
			} else {
				return false;
			}
		}
	}
	Table * targetTable = nullptr;
	{	// Find the table to bring the customer to
		for (Furniture * f : game.getFurniture()) {
			if (f->getType() == FurnitureType::TABLE) {
				Table * table = dynamic_cast<Table*>(f);
				if (table->customers.size() == 0) targetTable = table;
			}
		}
	}
	// Service the customer if a target table was found
	if (targetTable) {
		CustomerArriveEvent * event = static_cast<CustomerArriveEvent*>
			(game.getEvent(EventType::CUSTOMER_ARRIVE));
		// If a customer arrived, find a path to the table
		Vec2 tablePos = Vec2(targetTable->getX(), targetTable->getY());
		Vec2 target = game.findEmptyNearSquare(tablePos);
		currentPath = game.getPath(Vec2(x, y), target);
		// Update waiter state if a path was found
		if (currentPath.path.size() > 0) {
			state = WaiterState::ESCORT_CUSTOMER;
			pathIndex = 0;
			moveTimer.reset(true);
			// Setup the event to play once the waiter reaches its destination
			nextEvent = new WaiterSignalEvent{
				EventType::WAITER_SIGNAL,
				event->customer,
				this,
				targetTable };
			currentCustomer = event->customer;
			delete game.popEvent(EventType::CUSTOMER_ARRIVE);
			return true;
		}
	}
	return false;
}

bool Waiter::handleCustomerOrderEvent(Game & game) {
	CustomerOrderEvent * event = static_cast<CustomerOrderEvent*>
		(game.getEvent(EventType::CUSTOMER_ORDER));
	{	// Walk to the customer before taking the order
		bool adjacentToCustomer = false;
		if (std::abs(event->customer->getX() - x) + std::abs(event->customer->getY() - y) == 1) {
			adjacentToCustomer = true;
		}
		// If the waiter isn't adjacent to a stand, walk to one
		if (!adjacentToCustomer) {
			// TODO: actually find a path that's adjacent to the stand
			Vec2 target = game.findEmptyNearSquare(Vec2(event->customer->getX(), event->customer->getY()), 1);
			currentPath = game.getPath(Vec2(x, y), target);
			if (currentPath.path.size() > 0) {
				state = WaiterState::WALK_TO_CUSTOMER;
				pathIndex = 0;
				moveTimer.reset(true);
				return true;
			} else {
				return false;
			}
		}
	}
	Counter * targetCounter = nullptr;
	{	// Find the counter to bring the order to
		for (Furniture * f : game.getFurniture()) {
			if (f->getType() == FurnitureType::COUNTER) {
				Counter * counter = dynamic_cast<Counter*>(f);
				if (counter->num_orders <= 5) targetCounter = counter;
			}
		}
	}
	// Service the customer if a target table was found
	if (targetCounter) {
		// If a customer arrived, find a path to the table
		Vec2 counterPos = Vec2(targetCounter->getX(), targetCounter->getY());
		Vec2 target = game.findEmptyNearSquare(counterPos, 1, true);
		currentPath = game.getPath(Vec2(x, y), target);
		// Update waiter state if a path was found
		if (currentPath.path.size() > 0) {
			state = WaiterState::DELIVER_ORDER;
			pathIndex = 0;
			moveTimer.reset(true);
			// Setup the event to play once the waiter reaches its destination
			nextEvent = new OrderInEvent{
				EventType::ORDER_IN,
				event->customer,
				targetCounter
			};
			delete game.popEvent(EventType::CUSTOMER_ORDER);
			return true;
		}
	}
	return false;
}
