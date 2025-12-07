#include "Game.h"
#include <ctime>

int main() {
    srand(static_cast<unsigned>(time(0)));

    // creating the game engine
    Game game;

    // game loop
    // while the game runs
    while (game.running()) {
        // updating (moving stuff)
          game.update();
        // rendering stuff
        game.render();
    }

    return 0;
}
