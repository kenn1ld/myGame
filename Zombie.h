#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Enemy.h"

class Zombie : public Enemy {
public:
    Zombie();
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Override the base AI logic
    void handleIdle(float dt) override;
    void handleChase(float dt) override;
};

#endif
