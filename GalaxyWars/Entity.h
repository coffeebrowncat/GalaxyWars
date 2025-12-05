#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Entity {
protected: // only this class and its children can access the sprite
	// ENCAPSULATION
	Sprite sprite;

public:
	Entity() {} // constructor; runs once when the object is created

	// virtual destructor
	// RUBRIC: VIRTUAL DESTRUCTOR
	// virual to make sure that if you delete say a ship, it deletes the ships's destructor, not just this one
	virtual ~Entity() {}

	// pure virtual function
	// " = 0" means that the child must write its own version of this
	// RUBRIC: ABSTRACTION
	virtual void update() = 0;

	// render function
	// assuming every entity wants to simply draw its sprite
	virtual void render(RenderWindow& window) {
		window.draw(this->sprite);
	}

	// helper functions so other classes can ask "where are you?"
	// const to make sure no variables are changed
	const Vector2f& getPos() const { return this->sprite.getPosition(); }
	const FloatRect getBounds() const { return this->sprite.getGlobalBounds(); }
	void setPos(float x, float y) { this->sprite.setPosition(x, y); }
};