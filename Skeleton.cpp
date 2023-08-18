#include "Skeleton.h"

Skeleton::Skeleton() {
    // Initialize Skeleton attributes
    // Add Skeleton-specific attacks
    addAttack(Attack("Arrow Shot", 15.0f));
}

void Skeleton::update(float dt) {
    // Update Skeleton specific logic
}

void Skeleton::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Skeleton::handleChase(float dt) {
    // Skeleton-specific chase behavior
}

void Skeleton::handleAttack(float dt) {
    // Skeleton-specific attack behavior
}
