#pragma once
#include "PowerUp.h"
#include "Ship.h"

// class: RapidFire
// inherits from powerUp

using namespace sf;

class RapidFire : public PowerUp {
private:
	void initializeSprite() {
		static Texture texture;

		if (texture.getSize().x == 0) {
			if (!texture.loadFromFile("rapidFire.png")) {
				Image img;
				img.create(20, 20, Color::Magenta);
				texture.loadFromImage(img);
			}
		}
		// applying texture to the sprite
		this->sprite.setTexture(texture, true);
		this->sprite.setScale(1.f, 1.f);
	}

public:
	// constructor
	RapidFire(float x, float y) {
		this->initializeSprite();
		this->sprite.setPosition(x, y);
	}

	void update() {
		this->sprite.move(0.f, 2.f);
	}

	void applyEffect(Ship* player) {
		player->activateRapidFire();
	}
};