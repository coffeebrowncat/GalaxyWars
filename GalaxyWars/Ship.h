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

	// getters
	const int getHp() const { return this->hp; }
	const int getHpMax() const { return this->hpMax; }
};