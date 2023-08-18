#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <memory>  // Required for std::unique_ptr
#include "CollisionHandler.h"

class Player;  // Forward declaration of the Player class

class GameWorld {
private:
	CollisionHandler collisionHandler;
	std::vector<sf::Sprite> tiles;
	std::vector<sf::Texture> tileTextures;
	void loadTileLayer(const tmx::TileLayer& layer, const tmx::Map& map);
	sf::Vector2u tileSize;

	// Nested A* class
	class AStar {
	public:
		explicit AStar(const GameWorld& world);
		std::list<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const;

	private:
		struct Node {
			int x;
			int y;
			float cost = 0;
			float heuristic = 0;
			Node* parent = nullptr;  // Always initialize raw pointers

			Node(int x, int y) : x(x), y(y) {}  // Initialization done in-class, constructor is cleaner
			float totalCost() const { return cost + heuristic; }
		};

		using NodeList = std::vector<std::unique_ptr<Node>>;
		const GameWorld& world;
		NodeList getSuccessors(const Node* node) const;
		bool contains(const NodeList& list, int x, int y) const;
		std::list<sf::Vector2i> reconstructPath(Node* node) const;
	};

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