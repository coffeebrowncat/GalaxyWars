#include "Alien.h"
#include <cmath>

using namespace sf;

void Alien::initializeSprite() {
	// static to load once, all aliens share one image
	static Texture texture;

	if (texture.getSize().x == 0) {
		if (!texture.loadFromFile("enemyShip.png")) {
			Image img;
			img.create(50, 50, Color::Yellow);
			texture.loadFromImage(img);
		}
	}

	this->sprite.setTexture(texture, true);
	this->sprite.setScale(2.f, 2.f);
}

Alien::Alien(float x, float y, int type) {

	this->moveType = type;
	
	this->initializeSprite();
	// setting the starting position
	this->sprite.setPosition(x, y);

	// setting stats
	this->hpMax = 1;
	this->hp = this->hpMax;
	this->points = 10;
	this->damage = 1; 

	// speed based on type
	if (this->moveType == 0) this->speed = 3.f;      // slow (Level 1)
	else if (this->moveType == 1) this->speed = 5.f; // fast (Level 2)
	else this->speed = 4.f;                          // medium (Level 3 ZigZag)
}

Alien::~Alien() {}

void Alien::update() {
	// move 0 on x, and positive on y (down) 
	// level 1
	if (this->moveType == 0) {
		this->sprite.move(0.f, this->speed);
	}
	else if (this->moveType == 1) {
		this->sprite.move(1.f, this->speed);
	}
	else if (this->moveType == 2) {
		float wave = sin(this->sprite.getPosition().y * 0.03f) * 5.f;
		this->sprite.move(wave, this->speed);
	}	
}