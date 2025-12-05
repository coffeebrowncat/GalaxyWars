#include "Alien.h"

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

Alien::Alien(float x, float y) {
	this->initializeSprite();

	// setting the starting position
	this->sprite.setPosition(x, y);

	// setting stats
	this->speed = 3.f;
	this->hpMax = 1;
	this->hp = this->hpMax;
	this->points = 10;
}

Alien::~Alien() {}

void Alien::update() {
	// move 0 on x, and positive on y (down)
	this->sprite.move(0.f, this->speed);
}