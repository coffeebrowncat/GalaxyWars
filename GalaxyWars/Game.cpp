#include "Game.h"
#include <iostream>

using namespace std;

// initializing variables
void Game::initializeVariables() {
	this->window = nullptr; // intializing to prevent garbage values

	// creating the manager
	this->levelManager = new LevelManager();

	// initializing the bullet array
	this->maxBullets = 100;

	for (int i = 0; i < this->maxBullets; i++)
		this->bullets[i] = nullptr;

	// waiting roughly 10 frames between shots
	this->spawnTimerMax = 10.f;
	this->spawnTimer = this->spawnTimerMax;

	// enemy initialization
	this->maxEnemies = 50; // max of 50 enemies on screen at any given time

	for (int i = 0; i < this->maxEnemies; i++)
		this->enemies[i] = nullptr;

	this->enemySpawnTimerMax = this->levelManager->getSpawnTimeLimit();
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	// starting values for health and points
	this->points = 0;

	// initializing powerups
	for (int i = 0; i < 20; i++) {
		this->powerUps[i] = nullptr;
	}
}

// initializing window
void Game::initializeWindow() {
	// new reserving memory in ram (dma) (pls find empty ram and put this there)
	this->window = new RenderWindow(VideoMode(1600, 1200), "Galaxy Wars", Style::Titlebar | Style::Close);
	// limiting the game to fps to prevent breakage just incase
	this->window->setFramerateLimit(60);
}

// initializing player
void Game::initializePlayer() {
	this->player = new Ship(800.f, 1100.f); // bottom center positioning
}

void Game::spawnEnemy() {
	// finding any empty slot in the array
	for (int i = 0; i < this->maxEnemies; i++) {
		// if this slot is a nullptr, its free
		if (this->enemies[i] == nullptr) {
			// randomize X position (between 20 and 720 approx).
			// "rand() % 700" gives a random number from 0 to 699.
			float randomX = static_cast<float>(rand() % 1500 + 50);
			
			// chance logic
			int chance = this->levelManager->getAsteroidChance();

			// random number
			int roll = rand() % 100;

			if (roll < chance) {
				// creating the Alien dynamically 
				// y is -100 so it starts slightly above the screen and flies in.
				this->enemies[i] = new Asteroid(randomX, -100.f);
			}
			else {
				this->enemies[i] = new Alien (randomX, -100.f);
			}
			// spawning one enemy ONLY
			break;
		}
	}
}

// constructor
Game::Game() {
	this->initializeVariables();
	this->initializeWindow();
	this->initializePlayer();
}

// destructor
Game::~Game() {
	// necessary to use DELETE as we used NEW else memory leak occurs
	delete this->window;
	delete this->player;
	delete this->levelManager;

	// cleaning up the bullets
	// getting rid of any stray bullets when game closes
	for (int i = 0; i < this->maxBullets; i++) {
		// if the slot isn't empty
		if (this->bullets[i] != nullptr) {
			delete this->bullets[i]; // deleting the bullet object from the RAM
			this->bullets[i] = nullptr;
		}
	}

	// cleaning up and deleting enemies
	for (int i = 0; i < this->maxEnemies; i++) {
		if (this->enemies[i] != nullptr) {
			delete this->enemies[i];
		}
	}

	// deleting powerups
	for (int i = 0; i < 20; i++) {
		if (this->powerUps[i] != nullptr) {
			delete this->powerUps[i];
		}
	}
}

// checking if game runs
const bool Game::running() const {
	return this->window->isOpen();
}

// poll events (input handling)
void Game::pollEvents() {
	// asks "did anything happen since the last frame"
	while (this->window->pollEvent(this->e)) {
		// did user press the X button
		if (this->e.type == Event::Closed)
			this->window->close();
		// did user press esc
		if (this->e.type == Event::KeyPressed && this->e.key.code == Keyboard::Escape)
			this->window->close();

		// semi - automatic shooting
		// this only runs ONCE when the key goes down.
		if (this->e.type == Event::KeyPressed && this->e.key.code == Keyboard::Space) {

			// find empty seat
			for (int i = 0; i < this->maxBullets; i++) {
				if (this->bullets[i] == nullptr) {

					// create bullet
					this->bullets[i] = new Projectile(
						this->player->getPos().x - 23.f,
						this->player->getPos().y - 50.f,
						10.f
					);

					// stopping the loop
					break; 
				}
			}
		}
	}
}

void Game::updateCollision() {

	// need to check every enemy against every bullet
	for (int i = 0; i < this->maxEnemies; i++) {
		if (this->enemies[i] != nullptr) {
			// enemy vs bullet (check 1)
			for (int j = 0; j < this->maxBullets; j++) {
				if (this->bullets[j] != nullptr)
				{
					// getting the bounds of the enemy and the bullets and checking if they overlap
					if (this->enemies[i]->getBounds().intersects(this->bullets[j]->getBounds())) {
						// 1. delete bullet
						delete this->bullets[j];
						this->bullets[j] = nullptr;

						// 2. damage enemy (FIXED: Damage the ENEMY, not the player!)
						this->enemies[i]->takeDamage(1);

						// 3. check for death
						if (this->enemies[i]->getHp() <= 0) {

							// adding score
							this->points += this->enemies[i]->getPoints();
							cout << "Score: " << this->points << endl;

							// random loot drop logic
							int roll = rand() % 100;

							// tallying chances
							if (roll < 20) {
								// finding an empty slot in the powerUps array
								for (int p = 0; p < 20; p++) {
									if (this->powerUps[p] == nullptr) {
										// creating the healthpack exactly where the enemy dies
										this->powerUps[p] = new HealthPack(
											this->enemies[i]->getPos().x,
											this->enemies[i]->getPos().y
										);
										break;
									}
								}
							}

							// death
							delete this->enemies[i];
							this->enemies[i] = nullptr;
						}
						break; // bullet is used up
					}
				}
			}
		}
	}

	// enemy vs player (check 2)
	// looping through enemies again to see if they've collided with the ship
	for (int i = 0; i < this->maxEnemies; i++) {
		if (this->enemies[i] != nullptr) {
			// checking bounds
			if (this->enemies[i]->getBounds().intersects(this->player->getBounds())) {

				// confirm crash (FIXED: Use getDamage so Asteroids hit harder!)
				this->player->takeDamage(this->enemies[i]->getDamage());

				cout << "Health: " << this->player->getHp() << endl;
				delete this->enemies[i];
				this->enemies[i] = nullptr;

				// game over check
				if (this->player->getHp() <= 0) {
					cout << "GAME OVER" << endl;
					this->window->close();
				}
			}
		}
	}
}
// update (logic)
void Game::update() {
	this->pollEvents();

	// checking if the player exists
	if (this->player)
		this->player->update();

	// if the timer hasn't reached the max yet, add 1 to it.
	if (this->spawnTimer < this->spawnTimerMax)
		this->spawnTimer += 1.f;

	// condition 1: Is Space pressed?
	// condition 2: Is the timer ready? (spawnTimer >= spawnTimerMax)

	for (int i = 0; i < this->maxBullets; i++) {
		// taking into consideration slots that are occupied
		if (this->bullets[i] != nullptr) {
			// moving bullets
			this->bullets[i]->update();
			// checking if bullets went off screen
			if (this->bullets[i]->canDestroy()) {
				delete this->bullets[i]; // freeing the memory
				this->bullets[i] = nullptr;
			}
		}
	}

	// level logic
	// current score tracking
	this->levelManager->update(this->points);
	this->enemySpawnTimerMax = this->levelManager->getSpawnTimeLimit();

	// enemy timer update
	this->enemySpawnTimer += 1.f;

	// timer limit
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
		this->spawnEnemy(); // spawn a guy
		this->enemySpawnTimer = 0.f; // reset timer
	}

	//  moving enemies
	for (int i = 0; i < this->maxEnemies; i++) {
		// updating slots that have an enemy currently
		if (this->enemies[i] != nullptr) {
			this->enemies[i]->update(); // moving the enemy down
			// checking boundaries
			if (this->enemies[i]->getPos().y > 1200.f) {
				delete this->enemies[i];
				this->enemies[i] = nullptr;
			}
		}
	}

	this->updateCollision();

	// update powerups
	for (int i = 0; i < 20; i++) {
		if (this->powerUps[i] != nullptr) {
			this->powerUps[i]->update(); // move it

			// checking if the player touches it
			if (this->player->getBounds().intersects(this->powerUps[i]->getBounds())) {
				this->powerUps[i]->applyEffect(this->player);
				delete this->powerUps[i];
				this->powerUps[i] = nullptr;
			}
		}
	}
}

// render (drawing)
void Game::render() {
	// clearing old frame (wiping the screen)
	this->window->clear(Color(20, 20, 30));

	//drawing the new frame
	if (this->player)
		this->player->render(*this->window);

	// looping and drawing (bullets)
	for (int i = 0; i < this->maxBullets; i++) {
		// only drawing if there's a bullet
		if (this->bullets[i] != nullptr) {
			this->bullets[i]->render(*this->window);
		}
	}

	// looping and drawing enemies
	for (int i = 0; i < this->maxEnemies; i++) {
		// only drawing if the pointer is valid
		if (this->enemies[i] != nullptr) {
			this->enemies[i]->render(*this->window);
		}
	}

	// drawing powerups
	for (int i = 0; i < 20; i++) {
		if (this->powerUps[i] != nullptr) {
			// note: we need to cast or ensure PowerUp has a render function.
			// since PowerUp inherits Entity, and Entity has render(), this works!
			this->powerUps[i]->render(*this->window);
		}
	}

	this->window->display();
}