#ifndef ASTAR_H
#define ASTAR_H
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <vector>

class GameWorld;

class AStar {
public:
    explicit AStar(const GameWorld& world);
    std::list<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const;

private:
    struct Node {
        int x;
        int y;
        float cost = 0.0f;
        float heuristic = 0.0f;
        Node* parent = nullptr;

        Node(int x, int y) : x(x), y(y) {}

        float totalCost() const { return cost + heuristic; }
    };

    using NodeList = std::vector<std::unique_ptr<Node>>;

    NodeList getSuccessors(const Node* node) const;
    bool contains(const NodeList& list, int x, int y) const;
    std::list<sf::Vector2i> reconstructPath(Node* node) const;

    const GameWorld& world;
};

#endif // ASTAR_H