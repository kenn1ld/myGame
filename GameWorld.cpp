#include "GameWorld.h"
#include "Player.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Logger.h"
#include <algorithm>

void GameWorld::loadTileLayer(const tmx::TileLayer& layer, const tmx::Map& map) {
	const auto& tilesets = map.getTilesets();
	const auto& layerTiles = layer.getTiles();
	Logger::console->info("Loading tile layer with {} tiles.", layerTiles.size());
	const auto mapWidth = map.getTileCount().x;

	std::size_t tilesetIndex = 0;
	for (std::size_t i = 0; i < layerTiles.size(); ++i) {
		const auto& tile = layerTiles[i];
		if (tile.ID > 0) {
			tilesetIndex = -1;
			for (std::size_t j = 0; j < tilesets.size(); ++j) {
				if (tile.ID >= tilesets[j].getFirstGID() && tile.ID < tilesets[j].getLastGID()) {
					tilesetIndex = j;
					break;
				}
			}

			if (tilesetIndex == -1) {
				continue;
			}

			if (tile.ID == 67 || tile.ID == 83 || tile.ID == 104) {
				Logger::console->info("Processing tile ID: {}", tile.ID);
				Logger::console->info("Determined tileset index: {}", tilesetIndex);
			}

			if (tilesetIndex == tilesets.size()) {
				continue;
			}

			const auto& texture = tileTextures[tilesetIndex];
			sf::Sprite sprite;
			sprite.setTexture(texture);
			int tilesPerRow = static_cast<int>(texture.getSize().x) / static_cast<int>(map.getTileSize().x);
			sprite.setTextureRect({
				static_cast<int>((tile.ID - tilesets[tilesetIndex].getFirstGID()) % tilesPerRow * map.getTileSize().x),
				static_cast<int>((tile.ID - tilesets[tilesetIndex].getFirstGID()) / tilesPerRow * map.getTileSize().y),
				static_cast<int>(map.getTileSize().x),
				static_cast<int>(map.getTileSize().y)
				});

			auto tileWidth = map.getTileSize().x;
			auto tileHeight = map.getTileSize().y;

			float posX = static_cast<float>(i % mapWidth) * static_cast<float>(tileWidth);
			float posY = static_cast<float>(i / mapWidth) * static_cast<float>(tileHeight);

			sprite.setPosition(posX, posY);

			if (tile.ID == 67 || tile.ID == 83 || tile.ID == 104) {
				Logger::console->info("Texture Rect for tile ID {}: {}, {}, {}, {}",
					tile.ID, sprite.getTextureRect().left, sprite.getTextureRect().top,
					sprite.getTextureRect().width, sprite.getTextureRect().height);

				Logger::console->info("Sprite position for tile ID {}: {}, {}",
					tile.ID, sprite.getPosition().x, sprite.getPosition().y);
			}

			tiles.push_back(sprite);
		}
	}
}

GameWorld::GameWorld(const std::string& tmxPath) {
	tmx::Map map;
	if (!map.load(tmxPath)) {
		Logger::console->error("Error: Failed to load TMX map from {}", tmxPath);
		// handle error
	}

	auto& tmxTileSize = map.getTileSize();
	tileSize.x = tmxTileSize.x;
	tileSize.y = tmxTileSize.y;

	const auto& tilesets = map.getTilesets();
	Logger::console->info("Loading tile layer with {} tiles.", tilesets.size());
	for (const auto& ts : tilesets) {
		sf::Texture texture;
		if (!texture.loadFromFile(ts.getImagePath())) {
			// handle error
		}
		tileTextures.push_back(texture);
		Logger::console->info("Loaded tileset image from {}", ts.getImagePath());
	}

	const auto& layers = map.getLayers();
	for (const auto& layer : layers) {
		if (layer->getType() == tmx::Layer::Type::Tile) {
			loadTileLayer(*dynamic_cast<const tmx::TileLayer*>(layer.get()), map);
		}
	}
}

const std::vector<sf::Sprite>& GameWorld::getTiles() const {
	return tiles;
}

const CollisionHandler& GameWorld::getCollisionHandler() const {
	return collisionHandler;
}

void GameWorld::update(float dt, sf::View& view, const Player& player) const {
	// Center the camera on the player
	sf::Vector2f playerCenter = player.getPosition();
	view.setCenter(playerCenter);
	const float cameraSpeed = 200.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		view.move(0, -cameraSpeed * dt);
		Logger::console->info("Camera moving UP at speed {}", cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		view.move(0, cameraSpeed * dt);
		Logger::console->info("Camera moving DOWN at speed {}", cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		view.move(-cameraSpeed * dt, 0);
		Logger::console->info("Camera moving LEFT at speed {}", cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		view.move(cameraSpeed * dt, 0);
		Logger::console->info("Camera moving RIGHT at speed {}", cameraSpeed * dt);
	}
}

void GameWorld::draw(sf::RenderWindow& window, const sf::View& view) const {
	const float cullingMargin = 100.0f;
	sf::FloatRect viewBounds(
		view.getCenter() - view.getSize() * 0.5f - sf::Vector2f(cullingMargin, cullingMargin),
		view.getSize() + sf::Vector2f(2 * cullingMargin, 2 * cullingMargin)
	);

	int drawn_tiles_count = 0;
	for (const auto& tile : tiles) {
		if (tile.getGlobalBounds().intersects(viewBounds)) {
			window.draw(tile);
			drawn_tiles_count++;
		}
	}

	static int draw_counter = 0;
	draw_counter++;
	if (draw_counter % 60 == 0 && drawn_tiles_count > 0) {
		Logger::console->info("Drawn {} tiles in this frame.", drawn_tiles_count);
	}
}

bool GameWorld::checkCollision(const sf::FloatRect& rect) const {
	for (const auto& tile : tiles) {
		if (collisionHandler.checkCollision(tile.getGlobalBounds(), rect)) {
			Logger::console->info("Collision detected with tile.");
			return true;
		}
	}
	return false;
}

bool GameWorld::isGrounded(const sf::FloatRect& rect) const {
	sf::FloatRect movedRect = rect;
	movedRect.top += 1.0f;  // Small delta to check just below the player

	for (const auto& tile : tiles) {
		if (collisionHandler.checkCollision(tile.getGlobalBounds(), movedRect)) {
			if (static sf::Clock logClock; logClock.getElapsedTime().asSeconds() > 1.0f) {
				Logger::console->info("Object is grounded on a tile.");
				logClock.restart();
			}
			return true;
		}
	}
	return false;
}

bool GameWorld::isWalkable(int x, int y) const {
	sf::Vector2f position(static_cast<float>(x * tileSize.x), static_cast<float>(y * tileSize.y));

	return std::none_of(tiles.begin(), tiles.end(), [&position](const auto& tile) {
		return tile.getGlobalBounds().contains(position);
		});
}

sf::Vector2u GameWorld::getTileSize() const {
	return tileSize;
}

std::list<sf::Vector2i> GameWorld::findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const {
	AStar astar(*this);
	return astar.findPath(start, goal);
}

// A* class implementation

GameWorld::AStar::AStar(const GameWorld& world) : world(world) {}

std::list<sf::Vector2i> GameWorld::AStar::findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const {
	NodeList openList;
	NodeList closedList;
	auto startNode = std::make_unique<Node>(start.x, start.y);
	auto goalNode = std::make_unique<Node>(goal.x, goal.y);
	openList.emplace_back(std::move(startNode));

	while (!openList.empty()) {
		auto iter = std::min_element(openList.begin(), openList.end(), [](const auto& a, const auto& b) {
			return a->totalCost() < b->totalCost();
			});

		Node* currentNode = iter->get();

		if (currentNode->x == goalNode->x && currentNode->y == goalNode->y) {
			std::list<sf::Vector2i> path = reconstructPath(currentNode);
			return path;
		}

		closedList.emplace_back(std::move(*iter));
		openList.erase(iter);

		NodeList successors = getSuccessors(currentNode);

		for (auto& successor : successors) {
			if (contains(closedList, successor->x, successor->y)) {
				continue;
			}

			float deltaX = static_cast<float>(successor->x) - static_cast<float>(goalNode->x);
			float deltaY = static_cast<float>(successor->y) - static_cast<float>(goalNode->y);
			successor->heuristic = std::abs(deltaX) + std::abs(deltaY);

			float tempCost = currentNode->cost + 1;

			if (!contains(openList, successor->x, successor->y) || tempCost < successor->cost) {
				successor->parent = currentNode;
				successor->cost = tempCost;

				if (!contains(openList, successor->x, successor->y)) {
					openList.emplace_back(std::move(successor));
				}
			}
		}
	}

	return {};
}

GameWorld::AStar::NodeList GameWorld::AStar::getSuccessors(const Node* node) const {
	NodeList successors;
	std::vector<sf::Vector2i> moves = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	for (const auto& move : moves) {
		int newX = node->x + move.x;
		int newY = node->y + move.y;

		if (world.isWalkable(newX, newY)) {
			successors.emplace_back(std::make_unique<Node>(newX, newY));
		}
	}

	return successors;
}

bool GameWorld::AStar::contains(const NodeList& list, int x, int y) const {
	return std::find_if(list.begin(), list.end(), [x, y](const auto& node) {
		return node->x == x && node->y == y;
		}) != list.end();
}

std::list<sf::Vector2i> GameWorld::AStar::reconstructPath(Node* node) const {
	std::list<sf::Vector2i> path;
	while (node) {
		path.push_front({ node->x, node->y });
		node = node->parent;
	}
	return path;
}