#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Resource {
private:
	int value; // value of this particular resource
	Sprite sprite;
	Texture texture;

public:
	// constructor
	Resource(float x, float y, int val) {
		this->value = val;

		Image img;
		img.create(20, 20, Color::Cyan);
		this->texture.loadFromImage(img);
		this->sprite.setTexture(this->texture);
		this->sprite.setPosition(x, y);
	}

	// RUBRIC: OPERATOR OVERLOADING
	// syntax breakdown:
	// "Resource" - type of thing that we're returning
	// "operator+" - KEYWORD that tells cpp what to do when you see a plus sign
	// "(const Resource& other)": The thing on the right side of the plus sign.
	Resource operator+(const Resource& other) {
		// taking my value and the other value and summing them
		int combinedVal = this->value + other.value;

		// creating a brand new resource from the combined score
		// spawning at 0,0 bc its just a data object now
		Resource newResource(0, 0, combinedVal);
		return newResource;
	}

	int getValue() const { return this->value; }

	void render(RenderWindow& window) {
		window.draw(this->sprite);
	}
};