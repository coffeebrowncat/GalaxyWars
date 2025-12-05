#pragma once

class LevelManager {
private:
	int currentLevel;

	// difficulty settings
	float enemySpawnTimeLimit; // how fast the enemies spawn
	int asteroidChance; // % chance to spawn an asteroid (0 to 100)

public:
	LevelManager();
	~LevelManager();

	// passing the score to this function so it can decide whether or not to level up
	void update(unsigned int score);

	// getters
	const int getLevel() const { return this->currentLevel; }
	const float getSpawnTimeLimit() const { return this->enemySpawnTimeLimit; }
	const int getAsteroidChance() const { return this->asteroidChance; }
};