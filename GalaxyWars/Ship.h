#pragma once
#include "Entity.h" // reading the parent file to inherit from it
#include <SFML/Window/Keyboard.hpp>

// creating a class named ship that takes everything from "Entity"
// syntax indicating that the class ship (child) takes everything from entity (parent)
class Ship : public Entity {
private:
	// only ship can access these
	float movementSpeed;

	// health
	int hp;
	int hpMax;

	// shield
	int shieldTimer;
	bool hasShield;

	// rapid fire
	int rapidFireTimer;
	bool hasRapidFire;

	// private helpers to optimize code
	void initializeVariables();
	void initializeSprite();

public:
	Ship(float x, float y); // constructor takes x,y coordinates so we know ehre to spawn
	virtual ~Ship();

	// overriding the update function to add movement logic
	void update();

	void heal(int amount);
	void takeDamage(int damage);

	void activateShield() {
		this->hasShield = true;
		this->shieldTimer = 300;
	}
	void activateRapidFire() {
		this->hasRapidFire = true;
		this->rapidFireTimer = 300;
	}

	// getters
	const int getHp() const { return this->hp; }
	const int getHpMax() const { return this->hpMax; }
	bool getShieldState() const { return this->hasShield; }
	bool getRapidFireState() const { return this->hasRapidFire; }

	// timer logic
	void updatePowerups() {
		if (this->hasShield && --this->shieldTimer <= 0) this->hasShield = false;
		if (this->hasRapidFire && --this->rapidFireTimer <= 0) this->hasRapidFire = false;
	}
};