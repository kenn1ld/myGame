#include "Projectile.h"
#include <cmath>  // For std::sqrt

// Base Projectile Class
// Common constants for all projectiles
const float PROJECTILE_SPEED = 600.0f;
const sf::Color DIRECTION_LINE_COLOR = sf::Color::Red;

Projectile::Projectile(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition) {
	getMutableShape().setPosition(startPosition);
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
	initialVerticalSpeed = speed * direction.y;
}

void Projectile::update(float dt) {
	if (!isStuck) {
		// Apply physics
		initialVerticalSpeed += physics.gravity * dt;  // Apply gravity to the initial vertical speed

		// Move the projectile
		sf::Vector2f movement;
		movement.x = speed * direction.x * dt;
		movement.y = initialVerticalSpeed * dt;  // Use the modified vertical speed
		getMutableShape().move(movement);

		hitbox.left = getShape().getPosition().x;
		hitbox.top = getShape().getPosition().y;
		hitbox.width = getShape().getGlobalBounds().width;
		hitbox.height = getShape().getGlobalBounds().height;
	}
}

void Projectile::draw(sf::RenderWindow& window) const {
	window.draw(getShape());
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
	return viewBounds.intersects(getShape().getGlobalBounds());
}

// Derived GreenArrow Class from Projectile
const sf::Vector2f ARROW_SIZE(10.0f, 10.0f);
const sf::Color GREEN_ARROW_COLOR = sf::Color::Green;

GreenArrow::GreenArrow(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition)
	: Projectile(startPosition, targetPosition) {
	getMutableShape().setSize(ARROW_SIZE);  // Modified this line
	getMutableShape().setFillColor(GREEN_ARROW_COLOR);  // Modified this line
}

void GreenArrow::initializeAttributes() {
	getMutableShape().setSize(ARROW_SIZE);  // Modified this line
	getMutableShape().setFillColor(GREEN_ARROW_COLOR);  // Modified this line
}

// Derived RedArrow Class from Projectile
const sf::Color RED_ARROW_COLOR = sf::Color::Red;

RedArrow::RedArrow(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition)
	: Projectile(startPosition, targetPosition) {
	getMutableShape().setSize(ARROW_SIZE);  // Modified this line
	getMutableShape().setFillColor(RED_ARROW_COLOR);  // Modified this line
}

void RedArrow::initializeAttributes() {
	getMutableShape().setSize(ARROW_SIZE);  // Modified this line
	getMutableShape().setFillColor(RED_ARROW_COLOR);  // Modified this line
}