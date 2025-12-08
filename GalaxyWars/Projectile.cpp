#include "Projectile.h"

Projectile::Projectile(float x, float y, float speed) {
	this->speed = speed;
	this->destroy = false;

	// creating the projectile texture, static to load it only once
	// even if multiple bullets are fired, texture should load only once
	static Texture texture;

	// fallback
	if (texture.getSize().x == 0) {
		// small yellow rectangle (laser beam)
		Image img;
		img.create(15, 15, Color::Yellow);
		texture.loadFromImage(img);
	}
	
	// applying the texture to the sprite (inheriting from entity)
	// resetting sprite size to match image size (true)
	this->sprite.setTexture(texture, true);

	this->sprite.setPosition(x, y);
}

// destructor
Projectile::~Projectile() {}

// update logic
void Projectile::update() {
	// "move(x, y)": we move 0 on X, and negative on Y.
	// REMEMBER: in screens, Y=0 is the TOP. so negative Y means UP.
	this->sprite.move(0.f, -this->speed);

	// checking bounds
	if (this->sprite.getPosition().y < -20.f) {
		this->destroy = true; // marking for deletion 
	}
}