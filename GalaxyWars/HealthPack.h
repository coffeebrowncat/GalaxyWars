#pragma once
#include "PowerUp.h"
#include "Ship.h" // need to be able to see the ship class to heal it

using namespace sf;

class HealthPack : public PowerUp {
private:
	void initializeSprite() {
		static Texture texture;
		if (texture.getSize().x == 0) {
			if (!texture.loadFromFile("healthPack.png")) {
				Image img;
				img.create(30, 30, Color::Green);
				texture.loadFromImage(img);
			}
		}
		this->sprite.setTexture(texture, true);
		this->sprite.setScale(0.15f, 0.15f);
	}

public:
	// constructor
	HealthPack(float x, float y) {
		this->initializeSprite();
		this->sprite.setPosition(x, y);
	}

	// movement
	// overriding update because the functionality of powerups is different from ships
	void update() {
		// slow movement downward
		this->sprite.move(0.f, 2.f);
	}

	// rule implementaton
	// applying the applyeffect function when it comes to a healthpack
	void applyEffect(Ship* player) {
		player->heal(3); // healing 3 HP
	}
};