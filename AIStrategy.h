#ifndef AISTRATEGY_H
#define AISTRATEGY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "GameWorld.h"
class Skeleton;

class AIStrategy {
public:
	virtual ~AIStrategy() = default;
	virtual void execute(Skeleton& skeleton, float dt, const sf::Vector2f& playerPosition, const GameWorld& world) = 0;
};

class ChaseStrategy : public AIStrategy {
public:
	void execute(Skeleton& skeleton, float dt, const sf::Vector2f& playerPosition, const GameWorld& world)  override;
};

class AttackStrategy : public AIStrategy {
public:
	using AIStrategy::execute;
	void execute(Skeleton& skeleton, float dt, const sf::Vector2f& playerPosition, const GameWorld& world)  override;
};

#endif