#pragma once

#include <vector>

#include "furniture.hpp"

class Customer;

class Table : public Furniture {

public:

	Table(int x, int y);
	~Table();

	void render(int camX, int camY) override;
	void update() override;
	
	// List of references to customers sitting at the table
	std::vector<Customer*> customers;

private:

	static Texture * texture;

};