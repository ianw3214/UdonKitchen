#pragma once

#include "QcEngine.hpp"

enum class FurnitureType {
	STOVE,
	TABLE,
	CHAIR,
	COUNTER,
	STAND
};

class Furniture {

public:
	Furniture(FurnitureType type, int x, int y) : type(type), x(x), y(y) {}
	~Furniture() {}

	virtual void update() = 0;
	virtual void render(int camX, int camY) = 0;

	// Getter methods
	inline FurnitureType getType() const { return type; }
	inline int getX() const { return x; }
	inline int getY() const { return y; }

protected:
	// The coordinates of the furniture in grid coordinates
	int x, y;

private:
	FurnitureType type;
};