#pragma once

#include "furniture.hpp"

class Counter : public Furniture {

public:

	Counter(int x, int y);
	~Counter();

	void render(int camX, int camY) override;
	void update() override;

	int num_orders;

private:

	static Texture * texture;

};