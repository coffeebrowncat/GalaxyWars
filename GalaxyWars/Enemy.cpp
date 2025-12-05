#include "Enemy.h"

Enemy::Enemy() {
	// default stats
	this->hp = 1;
	this->hpMax = 1;
	this->damage = 1;
	this->points = 5;
}

Enemy::~Enemy() {
	// virtual destructor
	// deleting an enemy ensures 
}