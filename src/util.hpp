#pragma once

#include "QcEngine.hpp"

#define MOVE_TIME	100

// TODO: move variable definition to somewhere more global
#define BASE_TILE_WIDTH 64
#define BASE_TILE_HEIGHT 32

// struct to keep track of path when pathfinding
struct Path {
	std::vector<Vec2> path;
	int cost;
	int score;

	void operator=(const Path& other) {
		path = other.path;
		cost = other.cost;
		score = other.score;
	}
};

// equality operator for paths to compare if 2 paths are the same
inline bool operator==(const Path& p1, const Path& p2) {
	return p1.path == p2.path && p1.score == p2.score && p1.cost == p2.cost;
}

// temporary equality operator for vec2
// TODO: implement in engine
inline bool operator== (const Vec2& v1, const Vec2& v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

// helper function to convert from top down to iso coords
//  takes in coords as Tile coords, e.g. (1, 1) or (-3, 5) instead of pixel coords
//	returns iso coords in absolute pixel coords, e.g. (145, 65) or (-77, 46) instead of tiled coords
inline Vec2 isometric(const Vec2& vec) {
	int target_x = (vec.x * BASE_TILE_WIDTH) + (vec.y * BASE_TILE_WIDTH);
	int target_y = (vec.x * BASE_TILE_WIDTH / 2) - (vec.y * BASE_TILE_WIDTH / 2);
	return Vec2(target_x, target_y);
}

inline Vec2 isometric(int x, int y) {
	int target_x = (x * BASE_TILE_WIDTH) + (y * BASE_TILE_WIDTH);
	int target_y = (x * BASE_TILE_WIDTH / 2) - (y * BASE_TILE_WIDTH / 2);
	return Vec2(target_x, target_y);
}

// helper function to convert from top down to iso coords in RAW PIXEL UNITS
inline Vec2 isometric_raw(const Vec2& vec) {
	int target_x = vec.x + vec.y;
	int target_y = vec.x / 2 - vec.y / 2;
	return Vec2(target_x, target_y);
}

// helper function to convert between iso to top down coords
//	takes in iso coords in absolute pixel coords
//	returns cartesian coords in absolute pixel coords as well
inline Vec2 cartesian(const Vec2& vec) {
	int target_x = (vec.x + (vec.y * 2)) / 2;
	int target_y = (vec.x - (vec.y * 2)) / 2;
	return Vec2(target_x, target_y);
}

// helper function to get the width of the current window
inline int getWindowWidth() {
	int w;
	SDL_GetWindowSize(QcE::get_instance()->getWindow(), &w, NULL);
	return w;
}

// helper function to get the height of the current window
inline int getWindowHeight() {
	int h;
	SDL_GetWindowSize(QcE::get_instance()->getWindow(), NULL, &h);
	return h;
}