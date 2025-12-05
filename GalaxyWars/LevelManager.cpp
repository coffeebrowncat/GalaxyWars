#include "LevelManager.h"
#include <iostream>

using namespace std;

LevelManager::LevelManager() {
	// starting at level 1
	this->currentLevel = 1;
	this->enemySpawnTimeLimit = 80.f; // slow
	this->asteroidChance = 20; // 20% asteroids, 80% ships
}

LevelManager::~LevelManager(){}

void LevelManager::update(unsigned int score) {
	// hit a certain check point and upgrade
	// level 2
	if (this->currentLevel == 1 && score >= 50) {
		this->currentLevel = 2;
		this->enemySpawnTimeLimit = 60.f; // medium
		this->asteroidChance = 40; // 40% asteroids, 60% ships
		cout << "LEVEL UP! LEVEL 2!" << endl;
	}

	// level 3
	else if (this->currentLevel == 2 && score >= 150) {
		this->currentLevel = 3;
		this->enemySpawnTimeLimit = 40.f; // fast
		this->asteroidChance = 60; // 60% asteroids, 40% ships
		cout << "LEVEL UP! LEVEL 3!" << endl;
	}
}