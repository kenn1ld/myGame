#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <memory>
#include "CollisionHandler.h"

// Forward declarations
class AStar;
class Player;

class GameWorld {
private:
    CollisionHandler collisionHandler;
    std::vector<sf::Sprite> tiles;
    std::vector<sf::Texture> tileTextures;
    void loadTileLayer(const tmx::TileLayer& layer, const tmx::Map& map);
    sf::Vector2u tileSize;

    std::unique_ptr<AStar> astar;

public:
    explicit GameWorld(const std::string& tmxPath);
    void update(float dt, sf::View& view, const Player& player) const;
    void draw(sf::RenderWindow& window, const sf::View& view) const;
    bool checkCollision(const sf::FloatRect& rect) const;
    bool isGrounded(const sf::FloatRect& rect) const;
    const std::vector<sf::Sprite>& getTiles() const;
    const CollisionHandler& getCollisionHandler() const;
    bool isWalkable(int x, int y) const;
    std::list<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const;
    sf::Vector2u getTileSize() const;
};

#endif
