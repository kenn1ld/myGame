#include "Zombie.h"

Zombie::Zombie() {
    // Initialize Zombie attributes
    // Add Zombie-specific attacks
    addAttack(Attack("Swipe", 10.0f));
    addAttack(Attack("Bite", 20.0f));
}

void Zombie::update(float dt) {
    // Update Zombie specific logic
    // E.g., changing states based on proximity to the player
}

void Zombie::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Zombie::handleIdle(float dt) {
    // Zombie-specific idle behavior
}

void Zombie::handleChase(float dt) {
    // Zombie-specific chase behavior
}
