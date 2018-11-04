#pragma once

#include "QcEngine.hpp"

#include "staff.hpp"

class Waiter : public Staff {

public:
	Waiter(int x, int y);
	~Waiter();

	void update(Game& game) override;
	void render(int camX, int camY) override;

private:
	static Texture * texture;
};