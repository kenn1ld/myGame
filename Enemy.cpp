#include "Enemy.h"

void Enemy::takeDamage(float damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}

void Enemy::performAttack(size_t index) {
    if (index < attacks.size()) {
        attacks[index].execute();
        // Handle the attack logic, like reducing enemy mana, starting cooldown, etc.
    }
}

void Enemy::handleIdle(float dt) {
    // Base implementation of idle behavior
    // Can be overridden by derived classes
}

void Enemy::handlePatrol(float dt) {
    // Base implementation of patrol behavior
}

void Enemy::handleChase(float dt) {
    // Base implementation of chase behavior
}

void Enemy::handleAttack(float dt) {
    // Base implementation of attack behavior
}

void Enemy::handleFlee(float dt) {
    // Base implementation of flee behavior
}
