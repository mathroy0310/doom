#include <iostream>


#include "Engine/Game.h"

int main() {
    Engine::Game game;
    game.init();

    while (!game.isOver())
    {
        game.processInput();
        game.update();
        game.render();
        game.delay();
    }
	return 0;
}