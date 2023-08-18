#include "Skeleton.h"
#include <cstdlib>  // For rand() and RAND_MAX

Skeleton::Skeleton() {
	// Initialize Skeleton attributes
	sprite.setPosition(500, 0);  // Set the starting position
	sprite.setSize(sf::Vector2f(10, 10));  // Set the size of the square. You can adjust this as needed.
	sprite.setFillColor(sf::Color::Green);  // Set the color of the square to green
	velocity = sf::Vector2f(0, 0);
	// Add Skeleton-specific attacks
	addAttack(Attack("Arrow Shot", 15.0f));

	hitbox.setPosition(sprite.getPosition());
	hitbox.setSize(sprite.getSize());
	hitbox.setFillColor(sf::Color::Transparent);  // Transparent fill
	hitbox.setOutlineColor(sf::Color::Red);  // Red outline
	hitbox.setOutlineThickness(1.0f);  // Adjust thickness as needed

	// Initialize the AI vision rectangle
	aiVisionRect.setFillColor(sf::Color(255, 0, 0, 50));  // semi-transparent red
	aiVisionRect.setOutlineColor(sf::Color::Red);
	aiVisionRect.setOutlineThickness(1.0f);
}

void Skeleton::update(float dt, const GameWorld& world) {
	// Apply gravity to the vertical velocity
	velocity.y += GRAVITY * WEIGHT * dt;

	// Check if grounded
	isGrounded = world.isGrounded(hitbox.getGlobalBounds());
	if (isGrounded) {
		velocity.y = 0;  // Stop vertical movement if on the ground
		velocity.x *= (1 - FRICTION);  // Apply friction if grounded
	}
	else {
		// Apply air resistance if not grounded
		velocity.y *= (1 - AIR_RESISTANCE);
		velocity.x *= (1 - AIR_RESISTANCE);
	}

	moveAndHandleCollisions(dt, world);
}

void Skeleton::moveAndHandleCollisions(float dt, const GameWorld& world) {
	// Handle horizontal movement
	sf::Vector2f horizontalPredictedPosition = sprite.getPosition() + sf::Vector2f(velocity.x * dt, 0);
	hitbox.setPosition(horizontalPredictedPosition);

	while (true) {
		bool collisionDetected = false;
		sf::FloatRect hitboxBounds = hitbox.getGlobalBounds();
		for (const auto& tile : world.getTiles()) {
			if (world.getCollisionHandler().checkCollision(hitboxBounds, tile.getGlobalBounds())) {
				collisionDetected = true;
				world.getCollisionHandler().resolveCollision(hitboxBounds, tile.getGlobalBounds(), sf::Vector2f(velocity.x, 0));
				hitbox.setPosition(hitbox.getPosition() - sf::Vector2f(velocity.x * dt * 0.1f, 0));
				horizontalPredictedPosition = hitbox.getPosition();
			}
		}
		if (!collisionDetected) break;
		if (std::abs(velocity.x) < 0.1f) {
			velocity.x = 0;
			break;
		}
	}

	// Handle vertical movement based on the updated hitbox's position after horizontal movement
	sf::Vector2f verticalPredictedPosition = hitbox.getPosition() + sf::Vector2f(0, velocity.y * dt);
	hitbox.setPosition(verticalPredictedPosition);

	while (true) {
		bool collisionDetected = false;
		sf::FloatRect hitboxBounds = hitbox.getGlobalBounds();
		for (const auto& tile : world.getTiles()) {
			if (world.getCollisionHandler().checkCollision(hitboxBounds, tile.getGlobalBounds())) {
				collisionDetected = true;
				world.getCollisionHandler().resolveCollision(hitboxBounds, tile.getGlobalBounds(), sf::Vector2f(0, velocity.y));
				hitbox.setPosition(hitbox.getPosition() - sf::Vector2f(0, velocity.y * dt * 0.1f));
				verticalPredictedPosition = hitbox.getPosition();
			}
		}
		if (!collisionDetected) break;
		if (std::abs(velocity.y) < 0.1f) {
			velocity.y = 0;
			break;
		}
	}

	// Set the sprite's position to match the corrected hitbox's position
	sprite.setPosition(hitbox.getPosition());
}

void Skeleton::jump() {
	if (isGrounded) {
		velocity.y -= JUMP_FORCE;
	}
}

void Skeleton::handleChase(float dt, const sf::Vector2f& playerPosition, const GameWorld& world) {
	// Calculate the horizontal direction from the skeleton to the player
	float directionX = playerPosition.x - sprite.getPosition().x;

	// Normalize the direction
	directionX = (directionX > 0) ? 1 : ((directionX < 0) ? -1 : 0);

	// Define a chase speed
	float chaseSpeed = 5.0f;

	// Update the skeleton's horizontal velocity to move towards the player
	velocity.x = directionX * chaseSpeed;

	if (isGrounded) {
		bool shouldJump = false;

		// Define a check distance (how far ahead we should check for platforms)
		float checkDistance = 50.0f;  // This can be adjusted based on your game's scale

		// Check for obstacles or elevated platforms
		sf::FloatRect checkArea(
			sprite.getPosition().x + directionX * sprite.getSize().x,
			sprite.getPosition().y - sprite.getSize().y,  // Check a bit upwards
			directionX * checkDistance,
			2 * sprite.getSize().y  // Check a height of 2 times the skeleton's height
		);

		// Adjusted vertical vision for the AI to detect platforms above
		sf::FloatRect extendedCheckArea(
			sprite.getPosition().x,
			sprite.getPosition().y - 3 * sprite.getSize().y,  // Check higher upwards
			sprite.getSize().x,
			4 * sprite.getSize().y  // Check a height of 4 times the skeleton's height
		);

		for (const auto& tile : world.getTiles()) {
			if (world.getCollisionHandler().checkCollision(checkArea, tile.getGlobalBounds()) ||
				(world.getCollisionHandler().checkCollision(extendedCheckArea, tile.getGlobalBounds()) &&
					playerPosition.y < tile.getGlobalBounds().top + tile.getGlobalBounds().height)) {
				shouldJump = true;
				break;
			}
		}

		if (shouldJump) {
			jump();
		}

		aiVisionRect.setPosition(checkArea.left, checkArea.top);
		aiVisionRect.setSize(sf::Vector2f(checkArea.width, checkArea.height));
	}

	moveAndHandleCollisions(dt, world);
}





void Skeleton::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	window.draw(hitbox);
	window.draw(aiVisionRect);  // Draw the AI vision rectangle
}

void Skeleton::setStrategy(std::unique_ptr<AIStrategy> strategy) {
	aiStrategy = std::move(strategy);
}

void Skeleton::updateAI(float dt, const sf::Vector2f& playerPosition, const GameWorld& world) {
	if (aiStrategy) {
		aiStrategy->execute(*this, dt, playerPosition, world);  // Include the missing argument
	}
}