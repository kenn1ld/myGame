#include "Projectile.h"
#include <cmath>  // For std::sqrt

// Base Projectile Class
// Common constants for all projectiles
const float PROJECTILE_SPEED = 200.0f;
const sf::Color DIRECTION_LINE_COLOR = sf::Color::Red;

Projectile::Projectile(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition) {
    getShape().setPosition(startPosition);  // Use the accessor here
    speed = PROJECTILE_SPEED;

    // Compute direction vector
    sf::Vector2f diff = targetPosition - startPosition;
    float magnitude = std::hypot(diff.x, diff.y);
    direction = (magnitude != 0) ? diff / magnitude : sf::Vector2f(0.0f, 0.0f);

    directionLine.setPrimitiveType(sf::Lines);
    directionLine.resize(2);
    directionLine[0].position = startPosition;
    directionLine[1].position = targetPosition;
    directionLine[0].color = DIRECTION_LINE_COLOR;
    directionLine[1].color = DIRECTION_LINE_COLOR;
}

void Projectile::update(float dt) {
    getShape().move(speed * direction * dt);  // Use the accessor here
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(getShape());  // Use the accessor here
    window.draw(directionLine);
}

bool Projectile::isInsideScreen(const sf::RenderWindow& window) const {
    const sf::View& currentView = window.getView();
    sf::FloatRect viewBounds(
        currentView.getCenter().x - currentView.getSize().x / 2.0f,
        currentView.getCenter().y - currentView.getSize().y / 2.0f,
        currentView.getSize().x,
        currentView.getSize().y
    );
    return viewBounds.intersects(getShape().getGlobalBounds());  // Use the accessor here
}

// Derived GreenArrow Class from Projectile
const sf::Vector2f ARROW_SIZE(10.0f, 10.0f);
const sf::Color GREEN_ARROW_COLOR = sf::Color::Green;

GreenArrow::GreenArrow(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition)
    : Projectile(startPosition, targetPosition) {
    getShape().setSize(ARROW_SIZE);  // Use the accessor here
    getShape().setFillColor(GREEN_ARROW_COLOR);  // Use the accessor here
}

void GreenArrow::initializeAttributes() {
    getShape().setSize(ARROW_SIZE);  // Use the accessor here
    getShape().setFillColor(GREEN_ARROW_COLOR);  // Use the accessor here
}

// Derived RedArrow Class from Projectile
const sf::Color RED_ARROW_COLOR = sf::Color::Red;

RedArrow::RedArrow(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition)
    : Projectile(startPosition, targetPosition) {
    getShape().setSize(ARROW_SIZE);  // Use the accessor here
    getShape().setFillColor(RED_ARROW_COLOR);  // Use the accessor here
}

void RedArrow::initializeAttributes() {
    getShape().setSize(ARROW_SIZE);  // Use the accessor here
    getShape().setFillColor(RED_ARROW_COLOR);  // Use the accessor here
}
