#include "GameWorld.h"
#include "Player.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Logger.h"

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

			auto& texture = tileTextures[tilesetIndex];
			sf::Sprite sprite;
			sprite.setTexture(texture);
			int tilesPerRow = static_cast<int>(texture.getSize().x) / static_cast<int>(map.getTileSize().x);
			sprite.setTextureRect({
				static_cast<int>((tile.ID - tilesets[tilesetIndex].getFirstGID()) % tilesPerRow * map.getTileSize().x),
				static_cast<int>((tile.ID - tilesets[tilesetIndex].getFirstGID()) / tilesPerRow * map.getTileSize().y),
				static_cast<int>(map.getTileSize().x),
				static_cast<int>(map.getTileSize().y)
				});

			auto tileWidth = static_cast<unsigned int>(map.getTileSize().x);
			auto tileHeight = static_cast<unsigned int>(map.getTileSize().y);

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

void GameWorld::update(float dt, sf::View& view, Player& player) const {
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
		if (tile.getGlobalBounds().intersects(rect)) {
			Logger::console->info("Collision detected with tile.");
			return true;
		}
	}
	return false;
}

bool GameWorld::isGrounded(const sf::FloatRect& rect) const {
	sf::FloatRect movedRect = rect;
	movedRect.top += 1.0f; // Small delta to check just below the player

	for (const auto& tile : tiles) {
		if (tile.getGlobalBounds().intersects(movedRect)) {
			static sf::Clock logClock;
			if (logClock.getElapsedTime().asSeconds() > 1.0f) {
				Logger::console->info("Object is grounded on a tile.");
				logClock.restart();
			}
			return true;
		}
	}
	return false;
}