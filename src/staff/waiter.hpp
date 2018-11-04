#pragma once

#include "QcEngine.hpp"

#include "staff.hpp"
#include "../util.hpp"

enum class WaiterState {
	IDLE,
	WALK_TO_STAND,
	ESCORT_CUSTOMER,
	WAIT_FOR_CUSTOMER,
	WALK_TO_CUSTOMER,
	DELIVER_ORDER
};

struct Event;
class Customer;

class Waiter : public Staff {

public:
	Waiter(int x, int y);
	~Waiter();

	void update(Game& game) override;
	void render(int camX, int camY) override;

	void renderMapDebug(int camX, int camY) override;

	// ----- Getter methods -----
	inline WaiterState getState() const { return state; }

private:
	// Waiter state variables
	WaiterState state;
	Event * nextEvent;
	Customer * currentCustomer;

	// Movement variables
	Path currentPath;
	unsigned int pathIndex;
	Timer moveTimer;

	// ----- Helper methods -----
	bool findWaiterEvent(Game& game);
	bool handleCustomerArriveEvent(Game& game);
	bool handleCustomerOrderEvent(Game& game);

	// Textures
	static Texture * texture;
	static Texture * test;

};