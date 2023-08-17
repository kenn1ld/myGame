#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "GameWorld.h"
#include "Player.h"
#include "Logger.h"
// Above your main function or class declaration
constexpr float atk2FrameTime = 0.001f; // Adjust this value to change the animation speed

// Constructor
Player::Player(GameWorld& gw, sf::RenderWindow& win) : window(win), gameWorld(gw) {
	initializeSpriteProperties();
	initializePhysicsProperties();
	initializeHitboxProperties();
}

// ------------------- SPRITE INITIALIZATION -------------------

void Player::initializeSpriteProperties() {
	std::ifstream i("Elementals_leaf_ranger_free_v1.0.json");
	nlohmann::json spritesheet_data;
	i >> spritesheet_data;
	i.close();

	if (!texture.loadFromFile("Elementals_leaf_ranger_free_v1.0.png")) {
		// Handle error
	}
	sprite.setTexture(texture);
	sprite.setPosition(400, 0);

	animation.currentFrame = 0;
	animation.frameTime = 0.083f;
	animation.accumulator = 0.0f;

	std::vector<std::vector<sf::IntRect>> frames;
	frames.resize(7);

	for (int idx = 0; idx <= 11; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[0].push_back(frame);
	}

	for (int idx = 12; idx <= 21; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[1].push_back(frame);
	}

	for (int idx = 22; idx <= 33; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[2].push_back(frame);
	}

	for (int idx = 34; idx <= 39; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[3].push_back(frame);
	}

	for (int idx = 40; idx <= 43; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[4].push_back(frame);
	}

	for (int idx = 93; idx <= 102; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[5].push_back(frame);
	}

	for (int idx = 103; idx <= 117; idx++) {
		const auto& frame_data = spritesheet_data["frames"][idx]["frame"];
		sf::IntRect frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]);
		frames[6].push_back(frame);
	}

	animation.idleFrames = frames[0];
	animation.runFrames = frames[1];
	animation.jumpUpFrames = frames[2];
	animation.jumpDownFrames = frames[3];
	animation.landingFrames = frames[4];
	animation.atk1Frames = frames[5];
	animation.atk2Frames = frames[6];

	sprite.setTextureRect(animation.idleFrames[animation.currentFrame]);
}

// ------------------- PHYSICS INITIALIZATION -------------------

void Player::initializePhysicsProperties() {
	physics.speed = 100.0f;
	physics.verticalVelocity = 0.0f;
	physics.horizontalVelocity = 0.0f;
	physics.acceleration = 300.0f;
	physics.friction = 150.0f;
	physics.jumpForce = 400.0f;
	physics.airControl = 1.0f;
	physics.terminalVelocity = 400.0f;
	physics.elasticity = 0.5f;
	physics.maxHorizontalSpeed = 250.0f;
}

// ------------------- HITBOX INITIALIZATION -------------------

void Player::initializeHitboxProperties() {
	hitboxProps.offsetX = -15.0f;
	hitboxProps.offsetY = 20.0f;
	hitboxProps.hitboxWidth = 25.0f;
	hitboxProps.hitboxHeight = 40.0f;

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	sprite.setOrigin(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f);

	hitboxProps.hitbox = sf::FloatRect(spriteBounds.left + hitboxProps.offsetX, spriteBounds.top + hitboxProps.offsetY, hitboxProps.hitboxWidth, hitboxProps.hitboxHeight);
}

const sf::Sprite& Player::getSprite() const {
	return sprite;
}

const float Player::gravity = 9.8f;

// ------------------- PLAYER UPDATE AND DRAW -------------------

void Player::handleInput(float dt) {
	handleMovementInput(dt);  // Process movement inputs first
	applyGravity(dt);

	// Move shooting input check after movement and gravity
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isShooting = true;  // Set the shooting flag
		currentState = PlayerState::Atk2;
		animation.currentFrame = 0; // Start the animation from the beginning
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		isShooting = true;  // Set the shooting flag
		currentState = PlayerState::Atk1;
		animation.currentFrame = 0; // Start the animation from the beginning
	}

	updateAnimation(dt);
}

void Player::applyGravity(float dt) {
	if (!gameWorld.isGrounded(hitboxProps.hitbox)) {
		physics.verticalVelocity += gravity;
		physics.verticalVelocity = std::min(physics.verticalVelocity, physics.terminalVelocity);
	}
	else {
		physics.verticalVelocity = 0.0f;
	}
	moveVertically(dt);
}

void Player::moveVertically(float dt) {
	sprite.move(0, physics.verticalVelocity * dt);
	hitboxProps.hitbox.top += physics.verticalVelocity * dt;

	while (gameWorld.checkCollision(hitboxProps.hitbox)) {
		if (physics.verticalVelocity > 0) {
			// moving downwards
			sprite.move(0, -1.0f);
			hitboxProps.hitbox.top -= 1.0f;
		}
		else {
			// moving upwards
			sprite.move(0, 1.0f);
			hitboxProps.hitbox.top += 1.0f;

			// If moving upwards and hit a tile, immediately start falling
			physics.verticalVelocity = 0;  // You can set this to a small positive value if you want an immediate downward motion
		}
	}
}

void Player::spawnProjectile() {
	sf::Vector2f start_position;
	start_position.x = hitboxProps.hitbox.left + hitboxProps.hitbox.width;
	start_position.y = hitboxProps.hitbox.top + hitboxProps.hitbox.height / 2.0f;

	// Convert the window mouse coordinates to world coordinates
	sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
	sf::Vector2f targetPosition = window.mapPixelToCoords(mousePosInt); // Convert to world coordinates

	sf::Vector2f direction = targetPosition - start_position;

	// Normalize the direction vector
	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude != 0) {
		direction /= magnitude;
	}

	// Spawn the projectile
	projectiles.emplace_back(std::make_unique<RedArrow>(start_position, targetPosition));

}

void Player::updateProjectiles(float dt) {
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		(*it)->update(dt);  // Dereference the unique_ptr to access the update method
		if (!(*it)->isInsideScreen(window)) {
			Logger::console->info("Deleting projectile outside the view.");
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}
}

void Player::handleMovementInput(float dt) {
	float controlFactor = gameWorld.isGrounded(hitboxProps.hitbox) ? 1.0f : physics.airControl;

	// Jumping
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && gameWorld.isGrounded(hitboxProps.hitbox)) {
		physics.verticalVelocity = -physics.jumpForce;
		sprite.move(0, physics.verticalVelocity * dt);  // Directly move the sprite up
		hitboxProps.hitbox.top += physics.verticalVelocity * dt;    // Adjust the hitbox too
		currentState = PlayerState::Jump;       // Set the current state to Jump when jumping
	}

	if (!gameWorld.isGrounded(hitboxProps.hitbox) && physics.verticalVelocity > 0 && currentState != PlayerState::Atk1 && currentState != PlayerState::Atk2) {
		currentState = PlayerState::J_Down;
	}

	// Horizontal movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		physics.horizontalVelocity -= physics.acceleration * dt * controlFactor;
		sprite.setScale(-1.0f, 1.0f);          // Flip the sprite to face left
		if (gameWorld.isGrounded(hitboxProps.hitbox) && currentState != PlayerState::Atk1 && currentState != PlayerState::Atk2) {
			currentState = PlayerState::Running;   // Set the current state to Running ONLY if grounded and not in an attack state
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		physics.horizontalVelocity += physics.acceleration * dt * controlFactor;
		sprite.setScale(1.0f, 1.0f);           // Flip the sprite to face right
		if (gameWorld.isGrounded(hitboxProps.hitbox) && currentState != PlayerState::Atk1 && currentState != PlayerState::Atk2) {
			currentState = PlayerState::Running;   // Set the current state to Running ONLY if grounded and not in an attack state
		}
	}
	else if (currentState == PlayerState::Landing) {  // Check if in Landing state
		// Transition to Idle state only after landing animation is complete
		if (animation.currentFrame == animation.landingFrames.size() - 1) {
			currentState = PlayerState::Idle;
			animation.currentFrame = 0;
		}
	}
	else if (currentState == PlayerState::Atk1) {  // Check if in Atk1 state
		if (animation.currentFrame == animation.atk1Frames.size() - 1) {
			currentState = PlayerState::Idle;
			animation.currentFrame = 0;
		}
	}
	else if (currentState == PlayerState::Atk2) {  // Check if in Atk2 state
		if (animation.currentFrame == animation.atk2Frames.size() - 1) {
			currentState = PlayerState::Idle;
			animation.currentFrame = 0;
		}
	}
	else if (gameWorld.isGrounded(hitboxProps.hitbox)) {
		currentState = PlayerState::Idle;      // Default state to Idle ONLY if grounded and no movement keys are pressed
	}

	physics.horizontalVelocity = std::clamp(physics.horizontalVelocity, -physics.maxHorizontalSpeed, physics.maxHorizontalSpeed);
	applyFriction(dt);
	moveHorizontally(dt);
}

void Player::applyFriction(float dt) {
	if (physics.horizontalVelocity > 0) {
		physics.horizontalVelocity = std::max(0.0f, physics.horizontalVelocity - physics.friction * dt);
	}
	else if (physics.horizontalVelocity < 0) {
		physics.horizontalVelocity = std::min(0.0f, physics.horizontalVelocity + physics.friction * dt);
	}
}

void Player::moveHorizontally(float dt) {
	sprite.move(physics.horizontalVelocity * dt, 0);
	hitboxProps.hitbox.left += physics.horizontalVelocity * dt;

	while (gameWorld.checkCollision(hitboxProps.hitbox)) {
		if (physics.horizontalVelocity > 0) {
			// moving to the right
			sprite.move(-1.0f, 0);
			hitboxProps.hitbox.left -= 1.0f;
		}
		else {
			// moving to the left
			sprite.move(1.0f, 0);
			hitboxProps.hitbox.left += 1.0f;
		}
	}
}

void Player::updateSpriteDirection() {
	sf::Vector2i localMousePosition = sf::Mouse::getPosition(window); // Renamed variable
	sf::Vector2f targetPosition = window.mapPixelToCoords(localMousePosition); // Use the renamed variable
	bool isMouseOnLeft = targetPosition.x < sprite.getPosition().x;

	// Check if A or D keys are pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		sprite.setScale(-1.0f, 1.0f);          // Flip the sprite to face left
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		sprite.setScale(1.0f, 1.0f);           // Flip the sprite to face right
	}
	else {  // If no movement keys are pressed, use the mouse's direction
		if (isMouseOnLeft) {
			sprite.setScale(-1.0f, 1.0f); // Flip horizontally to face left
		}
		else {
			sprite.setScale(1.0f, 1.0f); // Flip horizontally to face right
		}
	}
}

bool Player::updateAnimationFrame([[maybe_unused]] float dt, const std::vector<sf::IntRect>& frames, float currentFrameTime) {
	if (animation.accumulator >= currentFrameTime) {
		animation.accumulator -= currentFrameTime;
		animation.currentFrame = (animation.currentFrame + 1) % frames.size();
		sprite.setTextureRect(frames[animation.currentFrame]);
		return true;
	}
	return false;
}

void Player::transitionToState(PlayerState newState) {
	currentState = newState;
	animation.currentFrame = 0;
	animation.accumulator = 0;
}

//Optimized

void Player::updateAnimation(float dt) {
	updateSpriteDirection();
	animation.accumulator += dt;

	switch (currentState) {
	case PlayerState::Idle:
		handleIdleState(dt);
		break;
	case PlayerState::Running:
		handleRunningState(dt);
		break;
	case PlayerState::Jump:
		handleJumpState(dt);
		break;
	case PlayerState::J_Down:
		handleJ_DownState(dt);
		break;
	case PlayerState::Landing:
		handleLandingState(dt);
		break;
	case PlayerState::Atk1:
		handleAtk1State(dt);
		break;
	case PlayerState::Atk2:
		handleAtk2State(dt);
		break;
		// ... [and similar code for the rest of the animations]
	}
}

void Player::handleIdleState(float dt) {
	updateAnimationFrame(dt, animation.idleFrames, animation.frameTime);
}

void Player::handleRunningState(float dt) {
	updateAnimationFrame(dt, animation.runFrames, animation.frameTime);
}

void Player::handleJumpState(float dt) {
	if (updateAnimationFrame(dt, animation.jumpUpFrames, animation.frameTime)) {
		if (animation.currentFrame == animation.jumpUpFrames.size() - 1) {
			transitionToState(PlayerState::J_Down);
		}
		if (gameWorld.isGrounded(hitboxProps.hitbox)) {
			transitionToState(PlayerState::Landing);
		}
	}
}

void Player::handleJ_DownState(float dt) {
	if (updateAnimationFrame(dt, animation.jumpDownFrames, animation.frameTime) &&
		(animation.currentFrame == animation.jumpDownFrames.size() - 1 || gameWorld.isGrounded(hitboxProps.hitbox))) {
		Logger::console->info("Player is grounded. Transitioning to Landing state.");
		transitionToState(PlayerState::Landing);
	}
}


void Player::handleLandingState(float dt) {
	if (updateAnimationFrame(dt, animation.landingFrames, animation.frameTime) && animation.currentFrame == animation.landingFrames.size() - 1) {
		transitionToState(PlayerState::Idle);
	}
}

void Player::handleAtk1State(float dt) {
	if (updateAnimationFrame(dt, animation.atk1Frames, animation.frameTime) && animation.currentFrame == animation.atk1Frames.size() - 1) {
		transitionToState(PlayerState::Idle);
	}
}

void Player::handleAtk2State(float dt) {
	if (updateAnimationFrame(dt, animation.atk2Frames, atk2FrameTime * 0.5)) {
		if (animation.currentFrame == 9) {
			spawnProjectile();
		}
		if (animation.currentFrame == animation.atk2Frames.size() - 1) {
			transitionToState(PlayerState::Idle);
		}
	}
}


sf::Vector2f Player::getPosition() const {
	return sprite.getPosition();
}

void Player::draw(sf::RenderWindow& renderWindow) const {
	renderWindow.draw(sprite);
	for (const auto& p_ptr : projectiles) {
		p_ptr->draw(renderWindow);
	}

	sf::RectangleShape redSquare(sf::Vector2f(hitboxProps.hitbox.width, hitboxProps.hitbox.height));
	redSquare.setPosition(hitboxProps.hitbox.left, hitboxProps.hitbox.top);
	redSquare.setFillColor(sf::Color::Transparent);
	redSquare.setOutlineColor(sf::Color::Red);
	redSquare.setOutlineThickness(2.0f);
	renderWindow.draw(redSquare);
}
