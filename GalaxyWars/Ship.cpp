#include "Ship.h"
#include <iostream>

using namespace std;
using namespace sf;

// :: scope resolution operator
// means "i am defining a function belonging to the Ship class
void Ship::initializeVariables() {
	// this -> pointer to the specific object we're working on
	// this -> movementSpeed - the movement speed variable of THIS particular ship
	this->movementSpeed = 5.f;
	this->hpMax = 10;
	this->hp = this->hpMax; // starting with full health
}

void Ship::initializeSprite() {
	// static to ensure that the variable is shared by ALL ships
	static Texture texture;

	if (texture.getSize().x == 0) { // Only do this once
		if (!texture.loadFromFile("ship.png")) {
			Image img;
			img.create(50, 50, Color::Magenta);
			texture.loadFromImage(img);
		}
	}

	// CRITICAL FIX: The "true" argument forces the sprite to update its rectangle
	this->sprite.setTexture(texture, true);

	// Safety: Force a color (White means "Show the texture as is")
	this->sprite.setColor(Color::White);
	this->sprite.setScale(2.f, 2.f);
	// Safety: Set origin to center
	this->sprite.setOrigin(25.f, 25.f);
}

// constructor logic
Ship::Ship(float x, float y) {
	// calling helper functions immediately as the ship is born

	cout << "DEBUG: Ship Constructor Running!" << endl;
	this->initializeVariables();
	this->initializeSprite();

	// setting the starting position
	this->sprite.setPosition(x, y);
}

// destructor logic
Ship::~Ship() {
	// to be filled
}

// update logic (runs 60x a second)
void Ship::update() {
	if (Keyboard::isKeyPressed(Keyboard::Left))
		this->sprite.move(-this->movementSpeed, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Right))
		this->sprite.move(this->movementSpeed, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Up))
		this->sprite.move(0.f, -this->movementSpeed);
	if (Keyboard::isKeyPressed(Keyboard::Down))
		this->sprite.move(0.f, this->movementSpeed);

	// checking bounds
	// get current position
	Vector2f pos = this->sprite.getPosition();

	// left wall (0 is the left edge)
	// We use "25" because the ship's origin is in the center (25 pixels from edge)
	if (pos.x < 25.f)
		this->sprite.setPosition(25.f, pos.y);

	// right wall (800 is the right edge)
	// 800 - 25 = 775
	else if (pos.x > 1575.f)
		this->sprite.setPosition(1575.f, pos.y);

	// top wall (0 is the top)
	if (pos.y < 25.f)
		this->sprite.setPosition(pos.x, 25.f);

	// bottom wall (600 is the bottom)
	// 600 - 25 = 575
	else if (pos.y > 1175.f)
		this->sprite.setPosition(pos.x, 1175.f);
}

void Ship::heal(int amount) {
	// adding health
	this->hp += amount;
	// 2. Debug print to see what the math did BEFORE the cap
	cout << "DEBUG: Math result: " << this->hp << endl;

	// capping health
	if (this->hp > this->hpMax) {
		this->hp = this->hpMax;
	}

	cout << "HEALED! HP IS NOW: " << this->hp << endl;
}

void Ship::takeDamage(int damage) {
	this->hp -= damage;
	if (this->hp < 0) this->hp = 0;
}