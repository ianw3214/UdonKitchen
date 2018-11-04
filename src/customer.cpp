#include "customer.hpp"

#include "game.hpp"

Texture * Customer::texture;
Texture * Customer::test;

Customer::Customer(int x, int y) : state(CustomerState::IDLE), x(x), y(y) {
	if (!texture) texture = new Texture("assets/customer.png");
	if (!test) test = new Texture("assets/test2.png");
}

Customer::~Customer() {

}

void Customer::update(Game& game) {
	switch (state) {
	case CustomerState::IDLE: {
		// If the customer is idle, find an event for him/her to do
		findCustomerEvent(game);
	} break;
	case CustomerState::WALK_TO_TABLE: {
		// Move the waiter once the timer time is up
		if (moveTimer.getTicks() > MOVE_TIME) {
			pathIndex++;
			x = currentPath.path[pathIndex].x;
			y = currentPath.path[pathIndex].y;
			// At this point, the customer is at the table
			if (pathIndex >= currentPath.path.size() - 1) {
				state = CustomerState::THINKING;
				currentPath = Path();
				stateTimer.reset(true);
			} else {
				moveTimer.reset(true);
			}
		}
	} break;
	case CustomerState::THINKING: {
		if (stateTimer.getTicks() > THINKING_TIME) {
			// Once the customer is done thinking, make an order
			game.pushEvent(new CustomerOrderEvent{EventType::CUSTOMER_ORDER, this});
			state = CustomerState::WAITING_FOR_FOOD;
		}
	} break;
	case CustomerState::WAITING_FOR_FOOD: {
		// TODO: IMPLEMENT THIS
	} break;
	default: {
		// Do nothing by default
	} break;
	}
}

void Customer::render(int camX, int camY) {
	Vec2 coords = isometric(x, y);
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	coords.y -= BASE_TILE_HEIGHT;
	texture->render(coords.x - camX, coords.y - camY);
}

void Customer::renderMapDebug(int camX, int camY) {
	// RENDER WEIRD THINGS
	for (const Vec2& v : currentPath.path) {
		Vec2 coords = isometric(v);
		coords.y -= BASE_TILE_HEIGHT;
		test->render(coords.x - camX, coords.y - camY);
	}
}

bool Customer::findCustomerEvent(Game & game) {
	// Find when a waiter has signaled the customer to go to a table
	// ASSUME THAT WHEN THE WAITER SIGNALS, THE TABLE IS VALID
	if (game.getEvent(EventType::WAITER_SIGNAL)) {
		WaiterSignalEvent * event = static_cast<WaiterSignalEvent*>
			(game.getEvent(EventType::WAITER_SIGNAL));
		// Add the customer to the table
		event->table->customers.push_back(this);
		// Find a chair adjacent to the table
		Chair * chair = nullptr;
		for (Furniture * f : game.getFurniture()) {
			if (f->getType() == FurnitureType::CHAIR) {
				if (std::abs(f->getX() - event->table->getX()) +
					std::abs(f->getY() - event->table->getY()) == 1)
				{
					chair = dynamic_cast<Chair*>(f);
				}
			}
		}
		// Move the player to the chair if possible
		if (chair) {
			state = CustomerState::WALK_TO_TABLE;
			Vec2 target = Vec2(chair->getX(), chair->getY());
			currentPath = game.getPath(Vec2(x, y), target, true);
			pathIndex = 0;
			moveTimer.reset(true);
			delete game.popEvent(EventType::WAITER_SIGNAL);
			return true;
		}
	}
	return false;
}
