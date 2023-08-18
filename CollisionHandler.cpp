#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() = default;

bool CollisionHandler::checkCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) const {
	return rect1.intersects(rect2);
}

void CollisionHandler::resolveCollision(sf::FloatRect& rect1, const sf::FloatRect& rect2, const sf::Vector2f& movementDirection) const {
	float overlapLeft{ rect1.left + rect1.width - rect2.left };
	float overlapRight{ rect2.left + rect2.width - rect1.left };
	float overlapTop{ rect1.top + rect1.height - rect2.top };
	float overlapBottom{ rect2.top + rect2.height - rect1.top };

	// Check horizontal collisions based on movement direction
	if (movementDirection.x > 0) { // Moving right
		if (overlapLeft < overlapTop && overlapLeft < overlapBottom) {
			rect1.left -= overlapLeft;
			return; // Exit after resolving
		}
	}
	else if (movementDirection.x < 0 && overlapRight < overlapTop && overlapRight < overlapBottom) { // Moving left
		rect1.left += overlapRight;
		return; // Exit after resolving
	}

	// Vertical collisions
	if (overlapTop < overlapRight && overlapTop < overlapLeft) {
		rect1.top -= overlapTop;
	}
	else {
		rect1.top += overlapBottom;
	}
}