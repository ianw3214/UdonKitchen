#pragma once

#include "QcEngine.hpp"

#include "event.hpp"

#define DEFAULT_MAP_SIZE	8
#define DEFAULT_ENTRANCE_X	0
#define DEFAULT_ENTRANCE_Y	2
// This function can only be used within Game class methods
#define MAP_INDEX(x, y) ((y) * map_size + (x))

#define MAX_CUSTOMERS		3

class Furniture;
class Staff;
class Customer;

#include "util.hpp"

// Wrapper to turn events into a linked list
struct EventWrapper {
	Event * event;
	EventWrapper * next;
};

class Game : public State {
public:
	Game();
	~Game();

	// State functions
	void init();
	void cleanup();
	void pause();
	void resume();
	void update();
	void render();

	// Getter methods for game state variables
	int getMapSize() const					{ return map_size; }
	int getEntranceX() const				{ return entrance_x; }
	int getEntranceY() const				{ return entrance_y; }
	std::vector<Furniture*>& getFurniture() { return furniture; }
	std::vector<Staff*>& getStaff()			{ return staff; }
	std::vector<Customer*>& getCustomers()	{ return customers; }

	// Event handling variables
	Event * getEvent(EventType type);
	Event * popEvent(EventType type);
	Event * pushEvent(Event * event);
	EventWrapper * getEventHead(EventType type);

	// Other utility variables
	Path getPath(Vec2 start, Vec2 end, bool chair_off = false);
	bool positionValid(Vec2 pos) const;
	Vec2 findEmptyNearSquare(Vec2 target, int range=2, bool square=false, int threshold=8);

private:
	// map state variables
	int map_size;
	int entrance_x;
	int entrance_y;

	// game entities
	std::vector<Furniture*> furniture;
	std::vector<Staff*> staff;
	std::vector<Customer*> customers;

	// game logic handlers
	void handleCustomerSpawns();
	
	// Event handling variables
	EventWrapper * events[NUM_EVENTS];

	// camera handling variables
	int camX, camY;
	int cursor_tile_x, cursor_tile_y;

	// Helper functions
	void getCursorTile();

	// Textures
	Texture * tile;
	Texture * hover;

	// Pathfinding methods
	bool findPath(Vec2 start, Vec2 end, Path& target, bool chair_off = false);
	bool pathfindHelper(Vec2 end, std::vector<Path>& open, std::vector<Vec2>& closed, Vec2 new_pos, const Path& shortest, Path& target);

	// ----- DEBUG METHODS -----
	void renderDebug();

	class DebugRenderer {
	public:
		DebugRenderer(Game& game, int x=0, int y=0, int font_size=20, std::string font="debug") : 
			game(game), 
			x(x), 
			current_y(y),
			font_size(font_size),
			font(font)
		{}

		inline void renderDebugString(const std::string& message, SDL_Color colour = { 255, 255, 255 }) {
			Texture * text = new Texture(game.getTextTexture(message, "debug", colour));
			text->render(x, current_y);
			current_y += font_size;
			delete text;
		}

	private:
		int x;
		int current_y;
		int font_size;
		std::string font;

		Game& game;
	};
};