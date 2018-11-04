#pragma once

class Game;

enum class StaffType {
	WAITER,
	CHEF
};

class Staff {

public:

	Staff(StaffType type, int x, int y) : type(type), x(x), y(y) {}
	~Staff() {}

	virtual void update(Game& game) = 0;
	virtual void render(int camX, int camY) = 0;

	// Getter methods
	inline StaffType getType() const { return type; }
	inline int getX() const { return x; }
	inline int getY() const { return y; }

protected:
	// The coordinates of the staff in grid coordinates
	int x, y;

private:
	StaffType type;
};