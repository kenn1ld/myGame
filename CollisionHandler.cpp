#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {}

bool CollisionHandler::checkCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) const {
    return rect1.intersects(rect2);
}

void CollisionHandler::resolveCollision(sf::FloatRect& rect1, const sf::FloatRect& rect2) const {
    // This is a basic resolution. More advanced resolutions can be based on penetration depth, etc.
    if (rect1.left < rect2.left) {
        rect1.left = rect2.left - rect1.width;
    }
    else {
        rect1.left = rect2.left + rect2.width;
    }

    if (rect1.top < rect2.top) {
        rect1.top = rect2.top - rect1.height;
    }
    else {
        rect1.top = rect2.top + rect2.height;
    }
}
