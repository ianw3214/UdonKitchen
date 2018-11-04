#include <iostream>

#include "QcEngine.hpp"
#include "game.hpp"

int main(int argc, char* argv[]) {

	QcEngine::createWindow("GMD", 1280, 720, false, NULL);
	QcEngine::setRates(60, 44);
	QcEngine::setState(std::make_unique<Game>());	

	while (QcEngine::isRunning()) {
		QcEngine::update();
	}

	return 0;

}