#ifndef ASTAR_H
#define ASTAR_H

#include <list>
#include <SFML/System/Vector2.hpp>

class GameWorld;  // Forward declaration

class AStar {
public:
    explicit AStar(const GameWorld& world);
    std::list<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const;

private:
    const GameWorld& gameWorld;
};

#endif /* ASTAR_H */
