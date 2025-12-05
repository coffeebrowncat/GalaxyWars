#include "Asteroid.h"

using namespace sf;

void Asteroid::initializeSprite() {
	static Texture texture;

	if (texture.getSize().x == 0) {
		if (!texture.loadFromFile("asteroid.png")) {
			Image img;
			img.create(50, 50, Color(100, 100, 100));
			texture.loadFromImage(img);
		}
	}

	this->sprite.setTexture(texture, true);
	this->sprite.setScale(0.5f, 0.5f);

	// setting rotation to center so that it rotates around its own axis
	this->sprite.setOrigin(25.f, 25.f);
}

Asteroid::Asteroid(float x, float y) {
	this->initializeSprite();
	this->sprite.setPosition(x, y);

	// stats
	this->speed = 5.f;
	this->rotationSpeed = 2.f;
	this->hpMax = 3.f; // takes 3 bullets to kill
	this->hp = this->hpMax;
	this->points = 20.f;
	this->damage = 2;
}

Asteroid::~Asteroid() {}

void Asteroid::update() {
	// moving down
	this->sprite.move(0.f, this->speed);
	// rotation
	this->sprite.rotate(this->rotationSpeed);
}