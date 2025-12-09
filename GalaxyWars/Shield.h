#pragma once
#include "PowerUp.h"
#include "Ship.h"

// class: shield
// inheritance: inherits from powerup, which inherits from entity
// RUBRIC: derived class && polymorphism

using namespace sf;

class Shield :public PowerUp {
private:
	void initializeSprite() {
		static Texture texture;
		
		if (texture.getSize().x == 0) {
			if (!texture.loadFromFile("shield.png")) {
				Image img;
				img.create(20, 20, Color::Cyan);
				texture.loadFromImage(img);
			}
		}
		// applying texture to the sprite
		this->sprite.setTexture(texture, true);
		this->sprite.setScale(0.75f, 0.75f);
	}

public:
	// constructor to spawn the item at a location (x,y) where the enemy died
	Shield(float x, float y) {
		this->initializeSprite();
		this->sprite.setPosition(x, y);
	}

	// overriding the update function
	// defines how item moves, slowly
	void update() {
		this->sprite.move(0.f, 2.f);
	}

	// overriding applyEffect function
	// specific logic for the shield
	// when player touches this we call activateShield
	void applyEffect(Ship* player) {
		player->activateShield();
	}
};