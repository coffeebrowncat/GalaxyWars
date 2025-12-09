#include "BossBullet.h"
#include <iostream>

using namespace sf;

void BossBullet::initializeSprite() {
    static Texture texture;

    // loading the shard
    if (texture.getSize().x == 0) {
        if (!texture.loadFromFile("shard.png")) {
            // fallback
            Image img;
            img.create(20, 60, Color::White);
            texture.loadFromImage(img);
        }
    }

    this->sprite.setTexture(texture, true);

    // scaling
    this->sprite.setScale(1.5f, 1.5f);

    // dimension handling
    FloatRect bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

BossBullet::BossBullet(float x, float y) {
    this->initializeSprite();
    this->sprite.setPosition(x, y);

    // stats
    this->speed = 6.f; // much faster than asteroids!
    this->hpMax = 1;   // one shot destroys it
    this->hp = 1;
    this->damage = 10;  // instant kill
    this->points = 5;  // tiny points
}

BossBullet::~BossBullet() {}

void BossBullet::update() {
    // moves straight down fast
    this->sprite.move(0.f, this->speed);
}

void BossBullet::render(sf::RenderTarget& target) {
    target.draw(this->sprite);
}