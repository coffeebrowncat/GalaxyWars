#pragma once
#include "Enemy.h"

// inheritance, asteroid is an enemy, which is an entity
// RUBRIC: INHERITANCE AND POLYMORPHISM
class Asteroid : public Enemy {
private:
	float speed;
	// rotating asteroid for shits and giggles
	float rotationSpeed;
	
	void initializeSprite();

public:
	// constructor
	Asteroid(float x, float y);
	virtual ~Asteroid();

	void update();
};