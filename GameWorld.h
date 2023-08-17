#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <vector>

class Player;  // Forward declaration of the Player class

class GameWorld {
private:
	std::vector<sf::Sprite> tiles;
	std::vector<sf::Texture> tileTextures;
	void loadTileLayer(const tmx::TileLayer& layer, const tmx::Map& map);

public:
	explicit GameWorld(const std::string& tmxPath);
	void update(float dt, sf::View& view, const Player& player) const;
	void draw(sf::RenderWindow& window, const sf::View& view) const;
	bool checkCollision(const sf::FloatRect& rect) const;
	bool isGrounded(const sf::FloatRect& rect) const;
};
#endif