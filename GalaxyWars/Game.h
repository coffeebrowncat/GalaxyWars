#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "Ship.h"
#include "Projectile.h"
#include "Alien.h"
#include "Asteroid.h"
#include "LevelManager.h"
#include "HealthPack.h"
#include "Shield.h"
#include "RapidFire.h"
#include "Boss.h"
#include "BossBullet.h"

using namespace sf;
using namespace std;

// RUBRIC: OPERATOR OVERLOADING
// defines a resource that can be added together using '+'
struct Resource {
	int amount;

	// constructor
	Resource(int v = 0) : amount(v) {}

	// RUBRIC: COPY CONSTRUCTOR (deep copy)
	Resource(const Resource& other) {
		this->amount = other.amount;
	}

	// allows us to: totalScrap = totalScrap + droppedScrap
	Resource operator+(const Resource& other) const {
		return Resource(this->amount + other.amount);
	}

	// helper to compare for upgrades
	bool operator >=(int threshold) const {
		return this->amount >= threshold;
	}

	// RUBRIC: FRIEND FUNCTION
	// allows us to cout << scrap directly
	friend ostream& operator << (ostream& out, const Resource& r) {
		out << r.amount;
		return out;
	}
};

// state machine enum
enum GameState { MENU, NAME_INPUT, PLAY, PAUSE, GAME_OVER, SCOREBOARD };

struct ScoreEntry {
	string name;
	int score;
	float timeSurvived;
};

class Game {
private:
	// using pointers to control exactly when the variables are created and destroyed
	RenderWindow* window;
	Event e;
	Font font;
	Font titleFont;
	Font outcomeFont;
	Font scoreFont;

	// menu background
	Texture menuBackgroundTexture;
	Sprite menuBackground;

	// name screen background
	Texture nameBackgroundTexture;
	Sprite nameBackground;

	// gameover 
	Texture outcomeBackgroundTexture;
	Sprite outcomeBackground;

	// scoreboard
	Texture scoreBackgroundTexture;
	Sprite scoreBackground;

	// background
	Texture gameBackgroundTexture;
	Sprite gameBackground;

	// player object
	Ship* player;

	// level manager
	LevelManager* levelManager;

	// bullet array
	// RUBRIC: DYNAMIC MEMORY
	Projectile* bullets[100];
	int maxBullets;

	// cooldown between shots
	float spawnTimer;
	float spawnTimerMax;

	// enemy array
	Enemy* enemies[50];
	int maxEnemies;

	// boss variables
	Boss* finalBoss;
	bool bossSpawned;

	float enemySpawnTimer;
	float enemySpawnTimerMax;

	// sound effects
	SoundBuffer shootBuffer;
	Sound shootSound;

	Music menuMusic;
	Music gameMusic;

	// points and health
	Resource scrap;
	GameState gameState;

	// storing a healthpack inside a PowerUp* slot (POLYMORPHISM)
	PowerUp* powerUps[20];

	// RUBRIC: FILE HANDLING - STORING TOP 50 SCORES
	// sizing it up to 51 to allow for a temp buffer when adding a new score
	ScoreEntry topScores[51];
	int numScores; // tracking how many scores exist

	Clock gameClock;
	float finalTime;

	// RUBRIC: STORE PLAYER NAME
	string playerName;
	Text nameInputText;

	// gui elements;
	Text guiText;
	Text menuText[3];
	Text gameOverText;
	Text scoreBoardText;
	Text levelUpText;
	Text pauseText;
	Text titleText;
	
	float levelUpTimer;
	int currentMenuOption;

	// intialization functions
	void initializeVariables();
	void initializeWindow();
	void initializePlayer();
	void initializeMenu();

	// helper function to spawn single enemy
	void spawnEnemy();

	// RUBRIC: ENCAPSULATION
	// state specific logic
	// these functions hide details of each state from the main loop
	void updateMenu(); // (Note: Menu logic is mostly input, handled in pollEvents)
	void updatePlay();

	void renderMenu();
	void renderPlay();
	void renderGameOver();
	void renderScoreBoard();

	// file handling helper functions
	void loadHighScores();
	void saveHighScores();
	// RUBRIC: SORTING
	void sortScores();

public:
	Game();
	virtual ~Game();

	// getters
	const bool running() const;

	// main functions
	void pollEvents();
	void updateCollision();
	void update();
	void render();
};