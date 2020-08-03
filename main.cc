#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "ConsoleView.h"
#include "GraphicsView.h"
#include "ViewManager.h"
#include "Game.h"
#include "Controller.h"
#include "Arguments.h"

int main(int argc, char* argv[])
{
	Arguments::parse(argc, argv);

	// set start level
	unsigned int startLevel = 0;
	if (Arguments::has("-startlevel"))
		startLevel = std::stoi(Arguments::getParam("-startlevel"));

	// seed PRNG std::rand
	if (Arguments::has("-seed")) {
		unsigned int seed = std::stoi(Arguments::getParam("-seed"));
		std::srand(seed);
	}

	Game game(startLevel, std::string("./sequence.txt"));
	Controller ctrl(game);
	ViewManager views;

	// add console view
	auto cv = std::make_shared<ConsoleView>(&game, &ctrl, std::cin, std::cout);
	views.push(cv);

	// set script file
	if (Arguments::has("-scriptfile")) {
		std::string script = Arguments::getParam("-scriptfile");
		cv->setScript(script);
	}

	// add graphics view
	if (!Arguments::has("-text")) {
		auto gv = std::make_shared<GraphicsView>("Quadris", &game, &ctrl);
		views.push(gv);
	}

	game.launch();
	views.launchViews();
	while (game.isRunning()) {
		ctrl.sendCommands();
		views.pollInput();
	}

	return 0;
}
