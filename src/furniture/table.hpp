#pragma once

#include "furniture.hpp"

class Table : public Furniture {

public:

	Table(int x, int y);
	~Table();

	void render(int camX, int camY) override;
	void update() override;

private:

	static Texture * texture;

};