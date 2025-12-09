#include <iostream>
#include <fstream>
#include <string>
#include "Game.h"
#include "RapidFire.h"
#include "HealthPack.h"
#include "Shield.h"
#include "GameException.h"


using namespace std;

// initializing variables
void Game::initializeVariables() {
	this->window = nullptr; // intializing to prevent garbage values

	// initializing the background
	if (!this->gameBackgroundTexture.loadFromFile("bg8.jpg")) {
		cout << "ERROR: COULD NOT LOAD BACKGROUND" << endl;
	}
	this->gameBackground.setTexture(this->gameBackgroundTexture);


	// Scale it to fit window if needed (optional)
	// this->worldBackground.setScale(...);

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
	this->scrap = Resource(0);

	// initializing powerups
	for (int i = 0; i < 20; i++) {
		this->powerUps[i] = nullptr;
	}

	// initializing the boss
	this->finalBoss = nullptr;
	this->bossSpawned = false;

	// start state
	this->gameState = MENU;
	this->currentMenuOption = 0;

	// initializing time for scores
	this->finalTime = 0.f;
	this->numScores = 0; // starting with 0 loaded scores

	// player name storage initialization
	this->playerName = ""; // starting empty
}

void Game::initializeMenu() {
	// robust resouce loading
	if (!this->font.loadFromFile("Aero.ttf")) {
		cout << "ERROR: FONT NOT FOUND!" << endl;
	}

	if (!this->titleFont.loadFromFile("Masterpiece.ttf")) {
		cout << "ERROR: FONT NOT FOUND!" << endl;
	}

	// gui setup
	this->guiText.setFont(this->font);
	this->guiText.setCharacterSize(24);
	this->guiText.setFillColor(Color::Yellow);
	this->guiText.setPosition(10.f, 10.f);

	// title
	this->titleText.setFont(this->titleFont);
	this->titleText.setString("Galaxy Wars");
	this->titleText.setCharacterSize(150); // Make it HUGE
	this->titleText.setFillColor(Color::White);

	// centering the title
	FloatRect titleRect = this->titleText.getLocalBounds();
	this->titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
	this->titleText.setPosition(800.f, 450.f);
	// menu options

	// start
	this->menuText[0].setFont(this->font);
	this->menuText[0].setString("START GAME");
	this->menuText[0].setCharacterSize(50);
	this->menuText[0].setFillColor(Color::Red); // default

	FloatRect r0 = this->menuText[0].getLocalBounds();
	this->menuText[0].setOrigin(r0.left + r0.width / 2.0f, r0.top + r0.height / 2.0f);
	this->menuText[0].setPosition(800.f, 600.f);

	// scoreboard
	this->menuText[1].setFont(this->font);
	this->menuText[1].setString("SCOREBOARD");
	this->menuText[1].setCharacterSize(50);
	this->menuText[1].setFillColor(Color::White);

	FloatRect r1 = this->menuText[1].getLocalBounds();
	this->menuText[1].setOrigin(r1.left + r1.width / 2.0f, r1.top + r1.height / 2.0f);
	this->menuText[1].setPosition(800.f, 700.f);

	// exit
	this->menuText[2].setFont(this->font);
	this->menuText[2].setString("EXIT");
	this->menuText[2].setCharacterSize(50);
	this->menuText[2].setFillColor(Color::White);

	FloatRect r2 = this->menuText[2].getLocalBounds();
	this->menuText[2].setOrigin(r2.left + r2.width / 2.0f, r2.top + r2.height / 2.0f);
	this->menuText[2].setPosition(800.f, 800.f);

	// gameover setup
	this->gameOverText.setFont(this->font);
	this->gameOverText.setString("GAME OVER\nPress R to Restart");
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setPosition(550.f, 400.f);
	this->gameOverText.setFillColor(Color::Red);

	// name input screen
	this->nameInputText.setFont(this->font);
	this->nameInputText.setCharacterSize(60);
	this->nameInputText.setFillColor(Color(235, 16, 136));
	this->nameInputText.setPosition(400.f, 500.f);
	this->nameInputText.setString("ENTER PILOT NAME:\n\n> _");

	FloatRect nameRect = this->nameInputText.getLocalBounds();
	this->nameInputText.setOrigin(nameRect.left + nameRect.width / 2.0f, nameRect.top + nameRect.height / 2.0f);
	this->nameInputText.setPosition(800.f, 600.f);

	// scoreboard setup
	this->scoreBoardText.setFont(this->font);
	this->scoreBoardText.setString("TOP 5 PILOTS\n\n1. ??? - 000\n2. ??? - 000\n3. ??? - 000\n\n(Press Esc to Return)");
	this->scoreBoardText.setCharacterSize(40);
	this->scoreBoardText.setPosition(600.f, 300.f);
	this->scoreBoardText.setFillColor(Color::Yellow);

	// level up text
	this->levelUpText.setFont(this->font);
	this->levelUpText.setCharacterSize(80); // BIG
	this->levelUpText.setFillColor(Color::Yellow);
	this->levelUpText.setString("LEVEL UP!");
	this->levelUpText.setPosition(600.f, 300.f); // roughly centered
	this->levelUpTimer = 0.f; // start hidden

	// pause menu
	this->pauseText.setFont(this->font);
	this->pauseText.setString("PAUSED\n\n[ESC] Resume\n[R] Restart\n[Q] Quit to Menu");
	this->pauseText.setCharacterSize(50);
	this->pauseText.setFillColor(Color(235, 16, 136));
	this->pauseText.setPosition(600.f, 400.f);
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
				if (this->scrap.amount > 300) {
					pattern = 2;
				}
				// if score is medium (>100), use Pattern 1 (Diagonal)
				else if (this->scrap.amount > 100) {
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
	this->initializeMenu();
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

	if (this->finalBoss != nullptr) {
		delete this->finalBoss;
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

		// oop: state specific routing
		if (this->gameState == MENU) {
			if (this->e.type == Event::KeyPressed) {
				if (this->e.key.code == Keyboard::Up) {
					this->currentMenuOption--;
					if (this->currentMenuOption < 0) this->currentMenuOption = 2; // wrap to bottom
				}
				else if (this->e.key.code == Keyboard::Down) {
					this->currentMenuOption++;
					if (this->currentMenuOption > 2) this->currentMenuOption = 0; // wrap to top
				}

				// updating visuals
				this->menuText[0].setFillColor(this->currentMenuOption == 0 ? Color::Red : Color::White);
				this->menuText[1].setFillColor(this->currentMenuOption == 1 ? Color::Red : Color::White);
				this->menuText[2].setFillColor(this->currentMenuOption == 2 ? Color::Red : Color::White);

				if (this->e.key.code == Keyboard::Enter) {
					if (this->currentMenuOption == 0) {
						this->gameState = NAME_INPUT;
						this->playerName = "";
					}
					else if (this->currentMenuOption == 1) {
						// >>> FIX START <<<
						this->gameState = SCOREBOARD;
						this->loadHighScores(); // Load only ONCE when entering state
						this->sortScores();     // Sort only ONCE
						// >>> FIX END <<<
					}
					else this->window->close();
				}
			}
		}

		// name_input state
		else if (this->gameState == NAME_INPUT) {

			// handling typing
			if (this->e.type == Event::TextEntered) {
				// 8 is ASCII for backspace
				if (this->e.text.unicode == 8) {
					if (!this->playerName.empty()) {
						this->playerName.pop_back();
					}
				}
				// limiting length to 10 chars and check standard ASCII
				else if (this->e.text.unicode < 128 && this->playerName.length() < 10) {
					this->playerName += static_cast<char>(this->e.text.unicode);
				}
			}

			// handling navigation
			if (this->e.type == Event::KeyPressed) {
				// press enter to confirm and start game
				if (this->e.key.code == Keyboard::Enter && !this->playerName.empty()) {
					this->gameState = PLAY;
				}
				// press escape to go back
				else if (this->e.key.code == Keyboard::Escape) {
					this->gameState = MENU;
				}
			}

			// updating UI immediately
			this->nameInputText.setString("ENTER PILOT NAME:\n\n> " + this->playerName + "_");
		}

		// pause menu logic
		else if (this->gameState == PAUSE) {
			if (this->e.type == Event::KeyPressed) {

				// resume
				if (this->e.key.code == Keyboard::Escape) {
					this->gameState = PLAY;
				}

				// restart 
				else if (this->e.key.code == Keyboard::R) {
					// reset logic (same as game over)
					this->scrap = Resource(0);
					delete this->player;
					this->initializePlayer();
					// wiping enemies/bullets
					for (int i = 0; i < this->maxEnemies; i++) {
						if (this->enemies[i]) { delete this->enemies[i]; this->enemies[i] = nullptr; }
					}
					if (this->finalBoss) { delete this->finalBoss; this->finalBoss = nullptr; }
					this->bossSpawned = false;

					this->gameState = PLAY;
				}

				// quitting to main menu
				else if (this->e.key.code == Keyboard::Q) {
					this->gameState = MENU;
				}
			}
		}

		else if (this->gameState == PLAY) {
			if (this->e.type == Event::KeyPressed && this->e.key.code == Keyboard::P) {
				this->gameState = PAUSE;
			}
		}

		else if (this->gameState == SCOREBOARD) {
			// if in scoreboard, esc goes back to menu
			if (this->e.type == Event::KeyPressed && this->e.key.code == Keyboard::Escape) {
				this->gameState = MENU;
			}
		}

		else if (this->gameState == GAME_OVER) {
			if (this->e.type == Event::KeyPressed && this->e.key.code == Keyboard::R) {

				// resetting Logic
				this->scrap = Resource(0);

				// // includes instructions to prevent ambiguity
				this->gameOverText.setString("GAME OVER\nSCORE: 0\n\n[R] Restart\n[S] Save Score");
				this->gameOverText.setFillColor(Color::Red);
				this->gameOverText.setPosition(500.f, 300.f); // recentering

				delete this->player;
				this->initializePlayer();

				// wiping enemies
				for (int i = 0; i < this->maxEnemies; i++) {
					if (this->enemies[i]) { delete this->enemies[i]; this->enemies[i] = nullptr; }
				}
				if (this->finalBoss) {
					delete this->finalBoss;
					this->finalBoss = nullptr;
				}
				this->bossSpawned = false;
				this->gameState = PLAY;
			}

			// inside GAME_OVER state
			else if (this->e.key.code == Keyboard::S) {

				// getting current text
				string s = this->gameOverText.getString();

				// check: only save if the word "SAVED" is NOT in the text yet
				if (s.find("SAVED") == string::npos) {
					try {
						this->saveHighScores();
						// visual feedback
						this->gameOverText.setString(s + "\n      (SAVED!)");
					}
					catch (const GameException& err) {
						cout << err.what() << endl;
					}
				}
			}
		}
	}
}

void Game::updateCollision() {
	// only run physics if playing
	if (this->gameState != PLAY) return;

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

							// RUBRIC: OPERATOR OVERLOADING
							// creating a "loot" resource from the enemy's value
							Resource loot(this->enemies[i]->getPoints());

							// combining resources using the overloaded '+' operator
							// satisfying the "r1 + r2" requirement
							this->scrap = this->scrap + loot;

							// debugging output (accessing .amount because it's a struct now)
							cout << "Scrap Resource: " << this->scrap.amount << endl;

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

				// confirm crash (using getDamage so that asteroids hit harder)
				this->player->takeDamage(this->enemies[i]->getDamage());

				cout << "Health: " << this->player->getHp() << endl;
				delete this->enemies[i];
				this->enemies[i] = nullptr;

				// game over check
				if (this->player->getHp() <= 0) {
					// RUBRIC: SCOREBOARD - CAPTURE TIME SURVIVED
					this->finalTime = this->gameClock.getElapsedTime().asSeconds();
					this->gameOverText.setString("GAME OVER\nSCORE: " + to_string(this->scrap.amount) + "\n\n[R] Restart\n[S] Save Score");
					this->gameOverText.setFillColor(Color::Red);
					this->gameState = GAME_OVER;
				}
			}
		}
	}

	// boss collision
	if (this->finalBoss != nullptr) {

		// check bullets hitting boss
		for (int j = 0; j < this->maxBullets; j++) {
			if (this->bullets[j] != nullptr) {
				if (this->finalBoss->getBounds().intersects(this->bullets[j]->getBounds())) {

					// deleting bullet
					delete this->bullets[j];
					this->bullets[j] = nullptr;

					// damaging Boss
					this->finalBoss->takeDamage(1);

					// checking for boss death
					if (this->finalBoss->getHp() <= 0) {
						// reward
						Resource loot(this->finalBoss->getPoints());
						this->scrap = this->scrap + loot;

						// victory logic
						// capture time survived
						this->finalTime = this->gameClock.getElapsedTime().asSeconds();

						// 1. changing text to GREEN Victory message
						this->gameOverText.setString("MISSION ACCOMPLISHED\nSCORE: " + to_string(this->scrap.amount) + "\n\n[R] Restart\n[S] Save Score");
						this->gameOverText.setFillColor(Color::Green);
						this->gameOverText.setPosition(400.f, 400.f);

						// 2. ending the game
						this->gameState = GAME_OVER;

						// 3. deleting Boss
						delete this->finalBoss;
						this->finalBoss = nullptr;

						// victory logic end
					}
					break;
				}
			}
		}

		// checking boss hitting player (Crash)
		if (this->finalBoss != nullptr && this->player != nullptr) {
			if (this->finalBoss->getBounds().intersects(this->player->getBounds())) {
				this->player->takeDamage(1); // drain health on contact

				// checking if player died from hitting boss
				if (this->player->getHp() <= 0) {
					this->finalTime = this->gameClock.getElapsedTime().asSeconds();
					this->gameState = GAME_OVER;
				}
			}
		}
	}
}

void Game::updatePlay() {
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
		// NORMAL: shoot ONLY if key is pressed NOW but was NOT pressed last frame (tap).
		if (isSpacePressed && !wasSpacePressed && this->spawnTimer >= this->spawnTimerMax) {
			readyToShoot = true;
		}
	}

	// execute shot
	if (readyToShoot) {

		// RUBRIC: UNLOCKING GREATER WEAPONS
		int weaponLevel = (this->scrap.amount >= 500) ? 2 : 1;

		for (int i = 0; i < this->maxBullets; i++) {
			if (this->bullets[i] == nullptr) {
				if (weaponLevel == 1) {
					// level 1: center shot
					this->bullets[i] = new Projectile(this->player->getPos().x, this->player->getPos().y - 20.f, 10.f);
				}
				else {
					// level 2: double shot
					this->bullets[i] = new Projectile(this->player->getPos().x - 25.f, this->player->getPos().y - 20.f, 10.f);
					// find next slot for 2nd bullet
					if (i + 1 < this->maxBullets) {
						this->bullets[i + 1] = new Projectile(this->player->getPos().x + 25.f, this->player->getPos().y - 20.f, 10.f);
					}
				}

				// reset timer (gun reload)
				this->spawnTimer = 0.f;

				// break to go back to firing one bullet at a time
				break;
			}
		}
	}

	// update memory for next frame
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

	// LEVEL TRANSITION LOGIC
	// 1. remember the old level
	int oldLevel = this->levelManager->getLevel();

	// 2. updating the manager
	this->levelManager->update(this->scrap.amount);

	// 3. did it change?
	if (this->levelManager->getLevel() > oldLevel) {
		// triggering the effect
		this->levelUpTimer = 180.f; // show for 3 seconds (60 frames * 3)
		this->levelUpText.setString("LEVEL " + to_string(this->levelManager->getLevel()));
	}

	// 4. update timer
	if (this->levelUpTimer > 0.f) {
		this->levelUpTimer -= 1.f;

		// simple blink: toggle color every 10 frames
		if ((int)this->levelUpTimer % 20 < 10)
			this->levelUpText.setFillColor(Color::Yellow);
		else
			this->levelUpText.setFillColor(Color::Red);
	}

	this->enemySpawnTimerMax = this->levelManager->getSpawnTimeLimit();
	this->enemySpawnTimerMax = this->levelManager->getSpawnTimeLimit();

	// enemy timer update
	this->enemySpawnTimer += 1.f;

	// timer limit
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {

		// forcing aliens to spawn here directly
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
					// 1. Get the OFFICIAL level from the manager
					int currentLevel = this->levelManager->getLevel();
					int pattern = 0; // Default: Straight (Level 1)

					// 2. Set Pattern based on Level
					if (currentLevel >= 3) {
						pattern = 2; // ZigZag (Hard)
					}
					else if (currentLevel == 2) {
						pattern = 1; // Diagonal (Medium)
					}

					// 3. Spawn
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

	// boss battle logic

	// 1. spawn check
	// if score is over 1000 and the boss is yet to spawn
	if (this->scrap.amount >= 1000 && !this->bossSpawned) {
		this->finalBoss = new Boss(700.f, -200.f);
		this->bossSpawned = true;
		cout << "WARNING: BOSS APPROACHING!" << endl;
	}

	// 2. logic if boss is ACTIVE
	if (this->finalBoss != nullptr) {

		// A. moving the boss
		this->finalBoss->update();

		// B. boss attack logic
		static float bossAttackTimer = 0.f;
		bossAttackTimer += 1.f;

		// attacking every 60 frames (approx 1 second)
		if (bossAttackTimer >= 60.f) {

			// finding a free slot in the enemy array
			for (int i = 0; i < this->maxEnemies; i++) {

				if (this->enemies[i] == nullptr) {

					// 1. calculating the dead center of the boss
					FloatRect bossBounds = this->finalBoss->getBounds();
					float centerX = bossBounds.left + (bossBounds.width / 2.f);
					float bottomY = bossBounds.top + bossBounds.height;

					// 2. spawning the shard there
					this->enemies[i] = new BossBullet(centerX, bottomY);

					break;
				}
			}
			bossAttackTimer = 0.f;
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

	// updating GUI text
	// display health + score
	this->guiText.setString("SCRAP: " + to_string(this->scrap.amount) + "\nHEALTH: " + to_string(this->player->getHp()));

	// right alignment logic
	// measuring the width of the text block every frame.
	FloatRect bounds = this->guiText.getLocalBounds();

	// screen width (1600) - text width - margin (30 pixels from edge)
	this->guiText.setPosition(1600.f - bounds.width - 30.f, 20.f);
}

void Game::renderPlay() {
	// drawing the game background first and foremost
	this->window->draw(this->gameBackground);

	// drawing the new frame
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

	// drawing boss
	if (this->finalBoss != nullptr) {
		this->finalBoss->render(*this->window);

		// boss health (Simple UI)
		float hpPercent = static_cast<float>(this->finalBoss->getHp()) / this->finalBoss->getMaxHp();

		// red bg
		RectangleShape barBack(Vector2f(300.f, 25.f));
		barBack.setPosition(650.f, 20.f);
		barBack.setFillColor(Color::Red);

		// green bg
		RectangleShape barFront(Vector2f(300.f * hpPercent, 25.f));
		barFront.setPosition(650.f, 20.f);
		barFront.setFillColor(Color::Green);

		this->window->draw(barBack);
		this->window->draw(barFront);
	}

	// drawing score
	this->window->draw(this->guiText);

	// drawing level up notification
	if (this->levelUpTimer > 0.f) {
		this->window->draw(this->levelUpText);
	}
}

void Game::update() {
	this->pollEvents(); // always handling input first (close window, menu navigation, etc)

	// OOP: state pattern implementation
	if (this->gameState == PLAY) {
		this->updatePlay();
	}
}

void Game::renderMenu() {
	this->window->draw(this->titleText);
	this->window->draw(this->menuText[0]);
	this->window->draw(this->menuText[1]);
	this->window->draw(this->menuText[2]);
}

void Game::renderGameOver() {
	// checking the color you set in updateCollision. 
		// red - dead, green - preen MUHAHAHAHHA
	bool victory = (this->gameOverText.getFillColor() == Color::Green);

	// black for both idc
	if (victory) {
		this->window->clear(Color(0, 0, 0));
	}
	else {
		this->window->clear(Color(0, 0, 0));
	}

	// SYSTEM FAILURE / MISSION COMPLETE)
	Text title;
	title.setFont(this->titleFont);
	title.setCharacterSize(110);

	if (victory) {
		title.setString("MISSION COMPLETE");
		title.setFillColor(Color::Cyan); // cyan 
	}
	else {
		title.setString("SYSTEM FAILURE");
		title.setFillColor(Color::Red);  // red

		// centering title
		FloatRect tBounds = title.getLocalBounds();
		title.setOrigin(tBounds.width / 2.f, tBounds.height / 2.f);
		title.setPosition(800.f, 300.f);
		this->window->draw(title);

		// drawing score
		Text scoreDisplay;
		scoreDisplay.setFont(this->font);
		scoreDisplay.setCharacterSize(60);
		scoreDisplay.setFillColor(Color::Yellow);
		scoreDisplay.setString("FINAL SCORE: " + to_string(this->scrap.amount));

		// centering score
		FloatRect sBounds = scoreDisplay.getLocalBounds();
		scoreDisplay.setOrigin(sBounds.width / 2.f, sBounds.height / 2.f);
		scoreDisplay.setPosition(800.f, 500.f);
		this->window->draw(scoreDisplay);

		// instructions
		Text instructions;
		instructions.setFont(this->font);
		instructions.setCharacterSize(40);
		instructions.setFillColor(Color::White);
		instructions.setString("PRESS [R] TO REBOOT SYSTEM\nPRESS [S] TO SAVE FLIGHT DATA");

		// centering
		FloatRect iBounds = instructions.getLocalBounds();
		instructions.setOrigin(iBounds.width / 2.f, iBounds.height / 2.f);
		instructions.setPosition(800.f, 700.f);
		this->window->draw(instructions);

		// save confirmation
		string oldString = this->gameOverText.getString();
		if (oldString.find("SAVED") != string::npos) {
			Text savedMsg;
			savedMsg.setFont(this->font);
			savedMsg.setString("> DATA UPLOADED SUCCESSFULLY <");
			savedMsg.setCharacterSize(30);
			savedMsg.setFillColor(Color(235, 16, 136));

			FloatRect svBounds = savedMsg.getLocalBounds();
			savedMsg.setOrigin(svBounds.width / 2.f, svBounds.height / 2.f);
			savedMsg.setPosition(800.f, 850.f);
			this->window->draw(savedMsg);
		}
	}
}

void Game::renderScoreBoard() {
	// drawing bg
	this->window->clear(Color(46, 0, 22));

	// drawing title
	Text title;
	title.setFont(this->titleFont); // Use the cool sci-fi font
	title.setString("HALL OF FAME");
	title.setCharacterSize(100);
	title.setFillColor(Color(235, 16, 136));

	// centering title
	FloatRect tBounds = title.getLocalBounds();
	title.setOrigin(tBounds.width / 2.f, tBounds.height / 2.f);
	title.setPosition(800.f, 200.f);
	this->window->draw(title);

	// looping through the top 5
	for (int i = 0; i < 5; i++) {
		// checking if we actually have a score at this index
		if (i < this->numScores) {
			Text entry;
			entry.setFont(this->font);
			entry.setCharacterSize(50);
			entry.setFillColor(Color::White);

			// number 1 player is gold
			if (i == 0) entry.setFillColor(Color::Yellow);

			// format
			string displayString = to_string(i + 1) + ". " +
				this->topScores[i].name + "   -   " +
				to_string(this->topScores[i].score);

			entry.setString(displayString);

			// centering each line
			FloatRect eBounds = entry.getLocalBounds();
			entry.setOrigin(eBounds.width / 2.f, eBounds.height / 2.f);
			entry.setPosition(800.f, 450.f + (i * 80.f)); // Spaced out nicely

			this->window->draw(entry);
		}
	}

	// returning instructions
	Text backText;
	backText.setFont(this->font);
	backText.setString("PRESS [ESC] TO RETURN");
	backText.setCharacterSize(30);
	backText.setFillColor(Color(100, 100, 100)); // Grey

	FloatRect bBounds = backText.getLocalBounds();
	backText.setOrigin(bBounds.width / 2.f, bBounds.height / 2.f);
	backText.setPosition(800.f, 900.f);

	this->window->draw(backText);

	// IMPORTANT: since we are in a render helper, we don't call display() here 
	// because the main render() function calls display() at the very end.
}

void Game::render() {
	// clearing old frame (wiping the screen)
	if (this->gameState == MENU) {
		this->window->clear(Color(46, 0, 22)); // menu, color // MAROON ISH
	}
	else {
		this->window->clear(Color(20, 20, 30)); // Game color
	}

	// OOP: rendering based on state
	if (this->gameState == MENU) {
		this->renderMenu();
	}
	else if (this->gameState == NAME_INPUT) {
		// clear to black
		this->window->clear(Color::Black);

		// 1. drawing a static title
		Text header;
		header.setFont(this->font);
		header.setString("IDENTIFY YOURSELF");
		header.setCharacterSize(100);
		header.setFillColor(Color(235, 16, 136));

		// center header
		FloatRect hBounds = header.getLocalBounds();
		header.setOrigin(hBounds.width / 2.f, hBounds.height / 2.f);
		header.setPosition(800.f, 300.f);
		this->window->draw(header);

		// 2. draw the Input (Name)
		// recenter the text every frame so it stays in the middle
		FloatRect nBounds = this->nameInputText.getLocalBounds();
		this->nameInputText.setOrigin(nBounds.width / 2.f, nBounds.height / 2.f);
		this->nameInputText.setPosition(800.f, 600.f);

		this->window->draw(this->nameInputText);
	}
	else if (this->gameState == PLAY) {
		this->renderPlay();
	}
	else if (this->gameState == PAUSE) {
		// 1. drawing the frozen game first
		this->renderPlay();

		// 2. drawing a semi-transparent black overlay (the dim effect)
		// 1600x1200 is your window size. (0,0,0, 200) = Black with high opacity
		RectangleShape overlay(Vector2f(1600.f, 1200.f));
		overlay.setFillColor(Color(0, 0, 0, 200));
		this->window->draw(overlay);

		// 3. drawing the pause text on top
		// making sure it's centered!
		FloatRect bounds = this->pauseText.getLocalBounds();
		this->pauseText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		this->pauseText.setPosition(800.f, 500.f);

		this->window->draw(this->pauseText);
	}
	else if (this->gameState == GAME_OVER) {
		this->renderGameOver();
	}
	else if (this->gameState == SCOREBOARD) {
		this->renderScoreBoard();
	}

	this->window->display();
}

void Game::loadHighScores() {
	this->numScores = 0; // reset counter

	// RUBRIC: EXCEPTION HANDLING - file errors
	// we try to open the file, if it doesn't exist, we just start fresh.
	ifstream inFile("scores.txt");

	if (inFile.is_open()) {
		// keep reading while we have room (max 50) and there is data
		while (this->numScores < 50 && !inFile.eof()) {
			std::string n;
			int s;
			float t;

			// reading format: name - score - time
			if (inFile >> n >> s >> t) {
				this->topScores[this->numScores].name = n;
				this->topScores[this->numScores].score = s;
				this->topScores[this->numScores].timeSurvived = t;
				this->numScores++;
			}
		}
		inFile.close();
	}
}

void Game::sortScores() {
	// RUBRIC: PROPER SORTING (descending order)
	// using bubble sort
	for (int i = 0; i < this->numScores - 1; i++) {
		for (int j = 0; j < this->numScores - i - 1; j++) {

			// checking if current score is LESS than the next score
			if (this->topScores[j].score < this->topScores[j + 1].score) {

				// swap Logic
				ScoreEntry temp = this->topScores[j];
				this->topScores[j] = this->topScores[j + 1];
				this->topScores[j + 1] = temp;
			}
		}
	}
}

void Game::saveHighScores() {
	// 1. loading existing data first so we append to history
	this->loadHighScores();

	// 2. adding the CURRENT game's result to the list
	// using the 51st slot (index numScores) as a temporary buffer
	if (this->numScores < 51) {

		// using the input name
		// if they somehow skipped input, default to "Unknown"
		if (this->playerName.empty()) this->playerName = "Unknown";

		this->topScores[this->numScores].name = this->playerName;
		this->topScores[this->numScores].score = this->scrap.amount;
		this->topScores[this->numScores].timeSurvived = this->finalTime;
		this->numScores++;
	}

	// 3. sorting everything (including the new score)
	this->sortScores();

	// 4. capping at 50 (discarding the 51st lowest score if full)
	if (this->numScores > 50) {
		this->numScores = 50;
	}

	// 5. writing to File
	// "trunc" mode deletes the old file content so we can write the fresh, sorted list
	ofstream outFile("scores.txt", ios::trunc);

	// RUBRIC: EXCEPTION HANDLING FOR FILE ERRORS 
	if (!outFile.is_open()) {
		// throwing our custom exception if we can't save
		throw GameException("CRITICAL ERROR: Unable to save scores to 'scores.txt'. Check disk permissions.");
	}

	for (int i = 0; i < this->numScores; i++) {

		// writing : name, score, time
		outFile << this->topScores[i].name << " "
			<< this->topScores[i].score << " "
			<< this->topScores[i].timeSurvived << endl;
	}

	outFile.close();
	cout << "High Scores Saved Successfully!" << endl;
}