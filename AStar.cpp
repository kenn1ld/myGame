#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <memory>
#include "GameWorld.h"

enum MovementType {
    WALKING,
    JUMPING
};

namespace std {
    template <>
    struct hash<sf::Vector2<int>> {
        std::size_t operator()(const sf::Vector2<int>& vec) const {
            return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
        }
    };
}

struct Node {
    sf::Vector2i position;
    int g;
    int h;
    int f;
    Node* parent;
    MovementType movementType;

    Node(sf::Vector2i pos, int g_val, int h_val, Node* par, MovementType moveType)
        : position(pos), g(g_val), h(h_val), f(g + h), parent(par), movementType(moveType) {}

    bool operator==(const Node& other) const {
        return position == other.position;
    }
};

struct NodeCompare {
    bool operator()(const Node* a, const Node* b) const {
        return a->f > b->f;
    }
};

AStar::AStar(const GameWorld& world) : gameWorld(world) {}

std::list<sf::Vector2i> AStar::findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const {
    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> openSet;
    std::unordered_map<sf::Vector2i, std::unique_ptr<Node>> allNodes;

    auto startNode = std::make_unique<Node>(start, 0, 0, nullptr, WALKING);
    Node* startNodeRaw = startNode.get();
    allNodes[start] = std::move(startNode);
    openSet.push(startNodeRaw);

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->position == goal) {
            std::list<sf::Vector2i> path;
            for (Node* temp = current; temp != nullptr; temp = temp->parent) {
                path.push_front(temp->position);
            }
            return path;
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;

                int x = current->position.x + dx;
                int y = current->position.y + dy;

                MovementType newMovementType = (dy == -1) ? JUMPING : WALKING;

                if (gameWorld.isWalkable(x, y)) {
                    int h = std::abs(x - goal.x) + std::abs(y - goal.y);

                    int cost = 1;
                    if (newMovementType != current->movementType) {
                        cost = 10;
                    }

                    auto neighbor = std::make_unique<Node>(sf::Vector2i(x, y), current->g + cost, h, current, newMovementType);

                    auto it = allNodes.find(neighbor->position);
                    if (it != allNodes.end()) {
                        Node* existingNeighbor = it->second.get();
                        if (neighbor->g >= existingNeighbor->g) continue;

                        existingNeighbor->g = neighbor->g;
                        existingNeighbor->h = neighbor->h;
                        existingNeighbor->f = existingNeighbor->g + existingNeighbor->h;
                        existingNeighbor->parent = current;
                        existingNeighbor->movementType = newMovementType;
                    }
                    else {
                        Node* neighborRaw = neighbor.get();
                        allNodes[neighbor->position] = std::move(neighbor);
                        openSet.push(neighborRaw);
                    }
                }
            }
        }
    }
    return {};
}
