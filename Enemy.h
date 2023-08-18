#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Attack.h"  // Include the Attack class

class Enemy {
protected:
    sf::Sprite sprite;
    float health;
    std::vector<Attack> attacks;  // List of attacks the enemy has

    // AI states
    enum class EnemyState {
        Idle,
        Patrol,
        Chase,
        Attack,
        Flee
    };

    EnemyState currentState = EnemyState::Idle;

public:
    virtual ~Enemy() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void takeDamage(float damageAmount);

    // Attack methods
    void addAttack(const Attack& attack) { attacks.push_back(attack); }
    void performAttack(size_t index);

    // Base AI logic (can be overridden)
    virtual void handleIdle(float dt);
    virtual void handlePatrol(float dt);
    virtual void handleChase(float dt);
    virtual void handleAttack(float dt);
    virtual void handleFlee(float dt);
};

#endif
