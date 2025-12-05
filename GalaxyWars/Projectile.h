#pragma once
#include "Entity.h"

// RUBRIC: INHERITANCE & POLYMORPHISM
class Projectile : public Entity {
private:
	float speed;
	bool destroy; // flag to determine deletion

public:
	// constructor
	Projectile(float x, float y, float speed);
	virtual ~Projectile();

	// overriding abstract function
	void update();

	// getter to check if bullet should be deleted
	const bool canDestroy() const { return this->destroy; }
};