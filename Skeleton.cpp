#include "Skeleton.h"

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
}



void Skeleton::moveAndHandleCollisions(float dt, const GameWorld& world) {
    // Update the skeleton's position based on its velocity
    sprite.move(velocity * dt);

    // Update hitbox 
    hitbox.setPosition(sprite.getPosition());

    // Check for collisions with tiles
    sf::FloatRect bounds = hitbox.getGlobalBounds();
    if (world.checkCollision(bounds)) {
        for (const auto& tile : world.getTiles()) {
            if (world.getCollisionHandler().checkCollision(tile.getGlobalBounds(), bounds)) {
                sf::FloatRect oldBounds = bounds;
                world.getCollisionHandler().resolveCollision(bounds, tile.getGlobalBounds());

                // Adjust velocities based on the collision resolution
                if (oldBounds.left != bounds.left) {
                    velocity.x = 0;
                }
                if (oldBounds.top != bounds.top) {
                    velocity.y = 0;
                }
                sprite.setPosition(hitbox.getPosition());
            }
        }
    }

    // Check if grounded
    if (world.isGrounded(hitbox.getGlobalBounds())) {
        velocity.y = 0;  // Stop vertical movement if on the ground
    }
}


void Skeleton::update(float dt, const GameWorld& world) {
    // Apply gravity to the vertical velocity
    velocity.y += GRAVITY * dt;

    moveAndHandleCollisions(dt, world);
}

void Skeleton::handleChase(float dt, const sf::Vector2f& playerPosition, const GameWorld& world) {
    // Calculate the horizontal direction from the skeleton to the player
    float directionX = playerPosition.x - sprite.getPosition().x;

    // Normalize the direction
    directionX = (directionX > 0) ? 1 : ((directionX < 0) ? -1 : 0);

    // Define a chase speed
    float chaseSpeed = 5.0f;  // You can adjust this value as needed

    // Update the skeleton's horizontal velocity to move towards the player
    velocity.x = directionX * chaseSpeed;

    moveAndHandleCollisions(dt, world);
}


void Skeleton::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hitbox);  // Draw the hitbox after the sprite
}

void Skeleton::setStrategy(std::unique_ptr<AIStrategy> strategy) {
    aiStrategy = std::move(strategy);
}

void Skeleton::updateAI(float dt, const sf::Vector2f& playerPosition, const GameWorld& world) {
    if (aiStrategy) {
        aiStrategy->execute(*this, dt, playerPosition, world);  // Include the missing argument
    }
}
