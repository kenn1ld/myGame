#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"

class Skeleton : public Enemy {
public:
    Skeleton();
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Override the base AI logic
    void handleChase(float dt) override;
    void handleAttack(float dt) override;
};

#endif
