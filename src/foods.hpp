#pragma once

#include <string>

struct Food {
	std::string name;
	unsigned int price;		// Price of the food
	unsigned int cost;		// Cost to make the food
	unsigned int time;		// Time it takes to cook the food

	std::string file_path;	// The file path to the food sprite
};

const Food BasicUdon{ "Basic Udon", 10, 5 , 2000, "assets/basic_udon.png"};