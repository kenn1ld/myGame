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
    static constexpr float AIR_RESISTANCE = 0.01f;  // Air resistance coefficient
    static constexpr float FRICTION = 0.1f;  // Friction coefficient on the ground
    static constexpr float WEIGHT = 70.0f;  // Weight of the skeleton in kg
    static constexpr float JUMP_FORCE = 500.0f;  // Jump force applied upwards

    sf::RectangleShape sprite;
    sf::RectangleShape hitbox;
    sf::RectangleShape aiVisionRect;
    bool isGrounded;  // Indicates if the skeleton is on the ground

    std::unique_ptr<AIStrategy> aiStrategy;  // Current AI strategy
    std::list<sf::Vector2i> currentPath;
    sf::Vector2i currentTargetNode;
    sf::Vector2f previousPlayerPosition;

    bool needsPathUpdate(const sf::Vector2f& playerPosition);

public:
    Skeleton();
    void update(float dt, const GameWorld& world) override;
    void draw(sf::RenderWindow& window) override;
    void updateAI(float dt, const sf::Vector2f& playerPosition, const GameWorld& world);
    void handleChase(float dt, const sf::Vector2f& playerPosition, const GameWorld& world) override;

    void setStrategy(std::unique_ptr<AIStrategy> strategy);
    void moveAndHandleCollisions(float dt, const GameWorld& world);
    void jump();  // New jump function
    void handlePathfinding(const sf::Vector2f& playerPosition, const GameWorld& world);
};

#endif
