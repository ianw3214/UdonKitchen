#pragma once

#include "furniture.hpp"

class Stove : public Furniture {

public:

	Stove(int x, int y);
	~Stove();

	void render(int camX, int camY) override;
	void update() override;

private:

	static Texture * texture;

};