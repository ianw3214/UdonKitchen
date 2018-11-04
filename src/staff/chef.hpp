#pragma once

#include "QcEngine.hpp"

#include "staff.hpp"

class Chef : public Staff {

public:
	Chef(int x, int y);
	~Chef();

	void update(Game& game) override;
	void render(int camX, int camY) override;

private:
	static Texture * texture;
};