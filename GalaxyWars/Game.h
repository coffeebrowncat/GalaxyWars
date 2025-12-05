#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"
#include "Projectile.h"
#include "Alien.h"
#include "Asteroid.h"
#include "LevelManager.h"
#include "Resource.h"
#include "HealthPack.h"

using namespace sf;

class Game {
private:
	// using pointers to control exactly when the variables are created and destroyed
	RenderWindow* window;
	Event e;

	// player object
	Ship* player;

	// level manager
	LevelManager* levelManager; // pointer to the level manager class
	
	// bullet array
	// storing pointers, not actual addresses
	// bullets[100] - creating a fixed array of 100 slots
	// RUBRIC: DYNAMIC MEMORY
	Projectile* bullets[100];
	int maxBullets;

	// cooldown between shots
	float spawnTimer;
	float spawnTimerMax;

	// enemy array
	Enemy* enemies[50];
	int maxEnemies;

	float enemySpawnTimer;
	float enemySpawnTimerMax;

	// points and health
	unsigned int points;

	// storing a healthpack inside a PowerUp* slot (POLYMORPHISM)
	PowerUp* powerUps[20];

	// intialization functions
	void initializeVariables();
	void initializeWindow();
	void initializePlayer();

	// helper function to spawn single enemy
	void spawnEnemy();

public:
	Game();
	virtual ~Game();

	// getters
	// const bool - returns yes/no without breaking anything
	const bool running() const;

	// main functions
	void pollEvents(); // keyboard/mouse input detection
	void updateCollision();
	void update(); // moving things
	void render(); // rendering	
};