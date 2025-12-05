#pragma once
#include "Entity.h"

// INHERITANCE: powerup is an entity (both position and sprite)

class PowerUp :public Entity {
public:
	PowerUp(){}
	virtual ~PowerUp(){}

	// pure virtual function
	// "virtual" - derived classes can alter this
	// returns nothing
	// applyEffect - name of the action
	// class Ship* player - needs to touch the player to work
	// "= 0": effect is still unknown. child class must define it.
	virtual void applyEffect(class Ship* player) = 0;
};