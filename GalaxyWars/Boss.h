#pragma once
#include "Enemy.h"
#include <cmath>

class Boss : public Enemy {
private:
	float speed;
	bool entering;

	void initializeSprite();

public:
	Boss(float x, float y);
	virtual ~Boss();

	void update();

	const int getMaxHp() const { return this->hpMax; }
};