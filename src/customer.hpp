#pragma once

#include "QcEngine.hpp"

class Customer {

public:

	Customer(int x, int y);
	~Customer();

	void update();
	void render(int camX, int camY);

	// Getter methods
	inline int getX() const { return x; }
	inline int getY() const { return y; }

private:
	// The coordinates of the staff in grid coordinates
	int x, y;

	static Texture * texture;
};