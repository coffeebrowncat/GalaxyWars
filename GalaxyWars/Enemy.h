#pragma once
#include "Entity.h"

// INHERITANCE (Enemy is a child of Entity)
// it gets position, sprite, and rendering for free

class Enemy : public Entity {
protected:
	int hp; // current health
	int hpMax; // max health
	int damage; // damage taken
	int points; // score

public:
	Enemy();
	virtual ~Enemy();

	// pure virtual function
	// forces the class to write its own logic instead of being generic - "alien" - "asteroid"
	virtual void update() = 0;

	// getters
	// these allow the Game Engine to read the stats safely.
	const int getDamage() const { return this->damage; }
	const int getHp() const { return this->hp; }
	const int getPoints() const { return this->points; }


	// we define this here so all enemies know how to get hurt.
	void takeDamage(int damage) {
		this->hp -= damage;
		if (this->hp < 0) this->hp = 0;
	}
};