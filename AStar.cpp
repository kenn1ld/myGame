#include "AStar.h"
#include "GameWorld.h"

// A* class implementation

AStar::AStar(const GameWorld& world) : world(world) {}

std::list<sf::Vector2i> AStar::findPath(const sf::Vector2i& start, const sf::Vector2i& goal) const {
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

AStar::NodeList AStar::getSuccessors(const Node* node) const {
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

bool AStar::contains(const NodeList& list, int x, int y) const {
	return std::find_if(list.begin(), list.end(), [x, y](const auto& node) {
		return node->x == x && node->y == y;
		}) != list.end();
}

std::list<sf::Vector2i> AStar::reconstructPath(Node* node) const {
	std::list<sf::Vector2i> path;
	while (node) {
		path.push_front({ node->x, node->y });
		node = node->parent;
	}
	return path;
}