#pragma once
#include "Enemy.h"

using namespace sf;

class BossBullet : public Enemy {
private:
    float speed;
    void initializeSprite();

public:
    BossBullet(float x, float y);
    virtual ~BossBullet();

    void update();
    virtual void render(RenderTarget& target);
};
