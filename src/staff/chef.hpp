#pragma once

#include "QcEngine.hpp"

#include "../util.hpp"

#include "staff.hpp"

#define PAUSE1_TIME		2000

class Stove;

enum class ChefState {
	IDLE,
	WALK_TO_COUNTER_FOR_ORDER,
	PAUSE1,
	WALK_TO_STOVE
};

class Chef : public Staff {

public:
	Chef(int x, int y);
	~Chef();

	void update(Game& game) override;
	void render(int camX, int camY) override;

	void renderMapDebug(int camX, int camY) override;

private:
	// Chef state variables
	ChefState state;
	Stove * stove;

	// Movement variables
	Path currentPath;
	unsigned int pathIndex;
	Timer moveTimer;

	// ----- Helper Methods -----
	bool findChefEvent(Game& game);
	bool handleOrderInEvent(Game& game);
	void findEmptyStovePath(Game& game);

	// textures
	static Texture * texture;
	static Texture * test;
};