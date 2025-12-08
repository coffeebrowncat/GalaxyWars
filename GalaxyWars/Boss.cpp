#include "Boss.h"

using namespace sf;

void Boss :: initializeSprite() {
	static Texture texture;

	if (texture.getSize().x == 0) {
		if (!texture.loadFromFile("boss.png")) {
			Image img;
			img.create(150, 100, Color::Yellow);
			texture.loadFromImage(img);
		}
	}

	this->sprite.setTexture(texture, true);
	this->sprite.setScale(2.0f, 2.0f); // making him BIG
}

Boss::Boss(float x, float y) {
	this->initializeSprite();
	this->sprite.setPosition(x, y);

	// stats
	this->hpMax = 100;
	this->hp = this->hpMax;
	this->damage = 50;
	this->points = 5000;

	this->speed = 3.f;
	this->entering = true; // starting off in entrance mode
}

Boss::~Boss(){}

void Boss::update() {
	// phase 1: entrance (flying down to the top of the arena)
	if (this->entering) {
		this->sprite.move(0.f, 2.f); // move down
		if (this->sprite.getPosition().y >= 50.f) {
			this->entering = false; // stop entering, start fighting
		}
	}

	// phase 2: combat
	else {
		this->sprite.move(this->speed, 0.f);
		// bounce off walls 
		if (this->sprite.getPosition().x <= 0.f) {
			this->speed = abs(this->speed); //  moving right
		}
		if (this->sprite.getPosition().x + this->sprite.getGlobalBounds().width >= 1600.f) {
			this->speed = -abs(this->speed); // moving left
		}
	}
}