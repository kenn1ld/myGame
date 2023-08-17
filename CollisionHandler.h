#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <SFML/Graphics.hpp>

class CollisionHandler {
public:
    CollisionHandler();

    // Check for a collision between two rectangles
    bool checkCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) const;

    // Resolve the collision by adjusting the position of the first rectangle
    void resolveCollision(sf::FloatRect& rect1, const sf::FloatRect& rect2) const;
};

#endif // COLLISION_HANDLER_H
