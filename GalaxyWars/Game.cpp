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
			int chance = 40;
			if (chance > 60) chance = 60; // Hard cap to ensure Aliens always have a 40% chance
			if (chance < 10) chance = 10; // Minimum asteroids

			// random number
			int roll = rand() % 100;



			if (roll < chance) {
				// creating the Alien dynamically 
				// y is -100 so it starts slightly above the screen and flies in.
				this->enemies[i] = new Asteroid(randomX, -100.f);
			}
			else {
				// enemy pattern logic
				// logic: 0 = Straight (Level 1), 1 = Diagonal (Level 2), 2 = ZigZag (Level 3)
				int pattern = 0;

				// if score is high (>300), use Pattern 2 (ZigZag)
				if (this->points > 300) {
					pattern = 2;
				}
				// if score is medium (>100), use Pattern 1 (Diagonal)
				else if (this->points > 100) {
					pattern = 1;
				}
				// otherwise default to 0 (Straight Down)

				// pass the 'pattern' variable to the Alien constructor!
				this->enemies[i] = new Alien(randomX, -100.f, pattern);
			}

			// spawning one enemy ONLY per tick
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
									// check: is this slot empty?
									if (this->powerUps[p] == nullptr) {
										// randomize drop type
										int type = rand() % 3;

										// creating the healthpack exactly where the enemy dies
										if (type == 0) {
											this->powerUps[p] = new HealthPack(
												this->enemies[i]->getPos().x,
												this->enemies[i]->getPos().y
											);
										}

										// shield invincibility
										else if (type == 1) {
											this->powerUps[p] = new Shield(
												this->enemies[i]->getPos().x,
												this->enemies[i]->getPos().y
											);
										}

										// rapid fire
										else {
											this->powerUps[p] = new RapidFire(
												this->enemies[i]->getPos().x,
												this->enemies[i]->getPos().y
											);
										}
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
	if (this->player) {
		this->player->update();
		// ensure timers count down!
		this->player->updatePowerups();
	}

	// shooting logic

	// if the timer hasn't reached the max yet, add 1 to it.
	if (this->spawnTimer < this->spawnTimerMax)
		this->spawnTimer += 1.f;

	// checking rapid fire state
	bool rapidFire = this->player->getRapidFireState();

	// if the player has the powerup equipped, set timer to 5 (fast), else 20
	this->spawnTimerMax = rapidFire ? 5.f : 9.f;

	// handling input "Memory" (did we press space last frame?)
	static bool wasSpacePressed = false;
	bool isSpacePressed = Keyboard::isKeyPressed(Keyboard::Space);

	// deciding if we should shoot
	bool readyToShoot = false;

	if (rapidFire) {
		// RAPID FIRE: shoot if key is held AND timer is ready.
		if (isSpacePressed && this->spawnTimer >= this->spawnTimerMax) {
			readyToShoot = true;
		}
	}
	else {
		// NORMAL: Shoot ONLY if key is pressed NOW but was NOT pressed last frame (tap).
		if (isSpacePressed && !wasSpacePressed && this->spawnTimer >= this->spawnTimerMax) {
			readyToShoot = true;
		}
	}

	// execute shot
	if (readyToShoot) {
		for (int i = 0; i < this->maxBullets; i++) {
			if (this->bullets[i] == nullptr) {
				// creating the bullet
				this->bullets[i] = new Projectile(
					this->player->getPos().x - 23.f,
					this->player->getPos().y - 50.f,
					10.f
				);

				// reset timer (gun reload)
				this->spawnTimer = 0.f;

				// break to go back to firing one bullet at a time
				break;
			}
		}
	}

	// Update memory for next frame
	wasSpacePressed = isSpacePressed;

	// bullet updates
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

		// >>> HARD FIX: Force Aliens to spawn directly here <<<
		for (int i = 0; i < this->maxEnemies; i++) {
			if (this->enemies[i] == nullptr) {

				float randomX = static_cast<float>(rand() % 1500 + 50);

				cout << "DEBUG: Spawning Alien at " << randomX << endl;
				// FORCE RATIO: 30% Asteroids, 70% Aliens
				int roll = rand() % 100;

				if (roll < 30) {
					// 30% Chance: Asteroid
					this->enemies[i] = new Asteroid(randomX, -100.f);
				}
				else {
					// 70% Chance: Alien!

					// >>> FIX 3: ADD ALL 3 LEVELS <<<
					int pattern = 0; // Default: Straight (Level 1)

					if (this->points > 400) {
						pattern = 2; // High Score: ZigZag (Level 3)
					}
					else if (this->points > 150) {
						pattern = 1; // Medium Score: Diagonal (Level 2)
					}

					this->enemies[i] = new Alien(randomX, -100.f, pattern);
				}
				break;
			}
		}

		this->enemySpawnTimer = 0.f; // reset timer
	}

	// moving enemies
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

			else if (this->powerUps[i]->getPos().y > 1200.f) {
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