#include "Game.h"
#include "GameException.h"
#include <ctime>
#include <iostream>

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));

    try {
        // RUBRIC: EXCEPTION HANDLING (TRY - CATCH USAGE)
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
    }

    catch (const GameException& e) {
        // RUBRIC: EXCEPTION HANDLING
        // this block catches custom errors (i.e: missing files)
        cout << "CRITICAL GAME ERROR: " << e.what() << endl;
        cout << "Press ENTER to EXIT..." << endl;
        cin.get(); // pausing so we can read the error
        return -1;
    }

    catch (const exception& e) {
        // catches standard cpp errors (like insufficient memory)
        cout << "STANDARD ERROR: " << e.what() << endl;
        return -2;
    }

    return 0;
}
