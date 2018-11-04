#pragma once

#include "QcEngine.hpp"

#include "util.hpp"

#define THINKING_TIME	2000

enum class CustomerState {
	IDLE,
	WALK_TO_TABLE,
	THINKING,
	WAITING_FOR_FOOD
};

class Game;

class Customer {

public:

	Customer(int x, int y);
	~Customer();

	void update(Game& game);
	void render(int camX, int camY);

	// Getter methods
	inline int getX() const { return x; }
	inline int getY() const { return y; }
	inline CustomerState getState() const { return state; }

	void renderMapDebug(int camX, int camY);

private:
	// The coordinates of the staff in grid coordinates
	int x, y;

	// State variables
	CustomerState state;
	Timer stateTimer;

	// Movement variables
	Path currentPath;
	unsigned int pathIndex;
	Timer moveTimer;

	// ----- Helper Methods -----
	bool findCustomerEvent(Game& game);

	// Textures
	static Texture * texture;
	static Texture * test;

};