#include "GameWorld.h"
#include "Player.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Logger.h"
#include <algorithm>
#include "AStar.h"

void GameWorld::loadTileLayer(const tmx::TileLayer& layer, const tmx::Map& map) {
    const auto& tilesets = map.getTilesets();
    const auto& layerTiles = layer.getTiles();
    const auto mapWidth = map.getTileCount().x;
    const auto& mapTileSize = map.getTileSize();
    const auto tileWidth = mapTileSize.x;
    const auto tileHeight = mapTileSize.y;

    std::size_t tilesetIndex = 0;

    // Mapping tile IDs to tileset indices for faster lookup
    std::unordered_map<std::size_t, std::size_t> tileToTilesetMap;
    for (std::size_t j = 0; j < tilesets.size(); ++j) {
        auto firstGID = tilesets[j].getFirstGID();
        auto lastGID = tilesets[j].getLastGID();
        for (auto id = firstGID; id < lastGID; ++id) {
            tileToTilesetMap[id] = j;
        }
    }

    for (std::size_t i = 0; i < layerTiles.size(); ++i) {
        const auto& tile = layerTiles[i];
        if (tile.ID <= 0) continue;

        auto it = tileToTilesetMap.find(tile.ID);
        if (it != tileToTilesetMap.end()) {
            tilesetIndex = it->second;
        }
        else {
            continue;
        }

        const auto& texture = tileTextures[tilesetIndex];
        // Use the exact type that is returned by the getSize() method
        auto tilesPerRow = texture.getSize().x / tileWidth;

        sf::Sprite sprite;
        sprite.setTexture(texture);

        // If you have to narrow it, use static_cast<int>
        sprite.setTextureRect({
            static_cast<int>((tile.ID - tilesets[tilesetIndex].getFirstGID()) % tilesPerRow * tileWidth),
            static_cast<int>((tile.ID - tilesets[tilesetIndex].getFirstGID()) / tilesPerRow * tileHeight),
            static_cast<int>(tileWidth),
            static_cast<int>(tileHeight)
            });

        sprite.setPosition(
            static_cast<float>(i % mapWidth) * tileWidth,
            static_cast<float>(i / mapWidth) * tileHeight
        );

        tiles.push_back(sprite);
    }
}

GameWorld::GameWorld(const std::string& tmxPath) {
    astar = std::make_unique<AStar>(*this);
    tmx::Map map;
    if (!map.load(tmxPath)) {
        Logger::console->error("Error: Failed to load TMX map from {}", tmxPath);
        // Handle error properly, perhaps by throwing an exception or initializing to a safe state.
        return;
    }

    // Cache frequently used values
    auto& tmxTileSize = map.getTileSize();
    tileSize = { tmxTileSize.x, tmxTileSize.y };

    const auto& tilesets = map.getTilesets();
    tileTextures.reserve(tilesets.size());  // Pre-allocate memory for textures

    for (const auto& ts : tilesets) {
        sf::Texture texture;
        if (!texture.loadFromFile(ts.getImagePath())) {
            Logger::console->error("Failed to load tileset image from {}", ts.getImagePath());
            continue;
        }
        tileTextures.emplace_back(texture);  // Use emplace_back for in-place construction
    }

    const auto& layers = map.getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            loadTileLayer(*dynamic_cast<const tmx::TileLayer*>(layer.get()), map);
        }
    }
}

// No need for changes; getter functions are already optimized.
const std::vector<sf::Sprite>& GameWorld::getTiles() const {
    return tiles;
}

const CollisionHandler& GameWorld::getCollisionHandler() const {
    return collisionHandler;
}

// Minimized logging and cached cameraSpeed as a const variable
void GameWorld::update(float dt, sf::View& view, const Player& player) const {
    const float cameraSpeed = 200.0f;
    view.setCenter(player.getPosition());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        view.move(0, -cameraSpeed * dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        view.move(0, cameraSpeed * dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        view.move(-cameraSpeed * dt, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        view.move(cameraSpeed * dt, 0);
    }
}

// Removed logging for drawn tiles; otherwise, the function is efficient.
void GameWorld::draw(sf::RenderWindow& window, const sf::View& view) const {
    const float cullingMargin = 100.0f;
    sf::FloatRect viewBounds(
        view.getCenter() - view.getSize() * 0.5f - sf::Vector2f(cullingMargin, cullingMargin),
        view.getSize() + sf::Vector2f(2 * cullingMargin, 2 * cullingMargin)
    );

    for (const auto& tile : tiles) {
        if (tile.getGlobalBounds().intersects(viewBounds)) {
            window.draw(tile);
        }
    }
}

// Removed logging for collision detection
bool GameWorld::checkCollision(const sf::FloatRect& rect) const {
    return std::any_of(tiles.begin(), tiles.end(), [this, &rect](const auto& tile) {
        return collisionHandler.checkCollision(tile.getGlobalBounds(), rect);
        });
}

// Removed excessive logging
bool GameWorld::isGrounded(const sf::FloatRect& rect) const {
    sf::FloatRect movedRect = rect;
    movedRect.top += 1.0f;

    return std::any_of(tiles.begin(), tiles.end(), [this, &movedRect](const auto& tile) {
        return collisionHandler.checkCollision(tile.getGlobalBounds(), movedRect);
        });
}

// No changes needed
bool GameWorld::isWalkable(int x, int y) const {
    sf::Vector2f position(static_cast<float>(x * tileSize.x), static_cast<float>(y * tileSize.y));
    return std::none_of(tiles.begin(), tiles.end(), [&position](const auto& tile) {
        return tile.getGlobalBounds().contains(position);
        });
}

// No changes needed
sf::Vector2u GameWorld::getTileSize() const {
    return tileSize;
}

// No changes needed
std::list<sf::Vector2i> GameWorld::findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const {
    AStar astar(*this);
    return astar.findPath(start, goal);
}
