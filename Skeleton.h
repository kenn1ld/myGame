// Skeleton.h

#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"
#include "AIStrategy.h"
#include <memory>
#include "GameWorld.h"
class AIStrategy;

class Skeleton : public Enemy {
private:
    sf::Vector2f velocity;
    static constexpr float GRAVITY = 9.81f;  // Gravity constant
    sf::RectangleShape sprite;
    sf::RectangleShape hitbox;

    std::unique_ptr<AIStrategy> aiStrategy;  // Current AI strategy

public:
    Skeleton();
    void update(float dt, const GameWorld& world) override;
    void draw(sf::RenderWindow& window) override;
    void updateAI(float dt, const sf::Vector2f& playerPosition, const GameWorld& world);
    // Override the base AI logic
    void handleChase(float dt, const sf::Vector2f& playerPosition, const GameWorld& world) override;

    void setStrategy(std::unique_ptr<AIStrategy> strategy);
    void moveAndHandleCollisions(float dt, const GameWorld& world);
};

#endif
