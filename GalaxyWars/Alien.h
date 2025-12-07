#pragma once
#include "Enemy.h"

// The Alien is a specific type of Enemy
class Alien : public Enemy {
private:
	float speed; // only the alien needs to know its own speed

	// helper function to handle the texture loading
	void initializeSprite();
	
	// patterned enemies
	int moveType;

public:
	Alien(float x, float y, int type);
	virtual ~Alien();

	// enemy class asks for movement so we define it
	void update();
};
