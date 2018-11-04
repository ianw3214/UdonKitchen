#pragma once

#include "furniture.hpp"

class Stand : public Furniture {

public:

	Stand(int x, int y);
	~Stand();

	void render(int camX, int camY) override;
	void update() override;

private:

	static Texture * texture;

};