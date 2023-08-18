#include "AIStrategy.h"
#include "Skeleton.h"

void ChaseStrategy::execute(Skeleton& skeleton, float dt, const sf::Vector2f& playerPosition, const GameWorld& world) {
	skeleton.handleChase(dt, playerPosition, world);
}
// Assuming there is a method named `handleAttack` in `Skeleton` class
void AttackStrategy::execute(Skeleton& skeleton, float dt, const sf::Vector2f& playerPosition, const GameWorld& world) {
	skeleton.handleAttack(dt);
}