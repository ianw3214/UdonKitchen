#pragma once

#include "furniture.hpp"

class Chair : public Furniture {

public:

	Chair(int x, int y);
	~Chair();

	void render(int camX, int camY) override;
	void update() override;

private:

	static Texture * texture;

};