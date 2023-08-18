#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameWorld.h"
#include "Projectile.h"
#include <memory>
#include "CollisionHandler.h"
enum class PlayerState {
    Idle,
    Running,
    Jump,
    J_Up,
    J_Down,
    Air_Atk_Sample,
    Air_Atk,
    Roll,
    Slide,
    Slide_Loop,
    Atk1,
    Atk2,
    Atk2_Entangle_Arrow_Sample,
    Atk2_Poison_Arrow_Sample,
    Atk3,
    Atk3_Sample_Ref,
    Sp_Atk,
    Defend,
    Take_Hit,
    Death,
    Landing
};

struct PhysicsProperties {
    float speed;
    float verticalVelocity;
    float horizontalVelocity;
    float acceleration;
    float friction;
    float jumpForce;
    float airControl;
    float terminalVelocity;
    float elasticity;
    float maxHorizontalSpeed;
    float maxVerticalSpeed;
};

struct AnimationProperties {
    int currentFrame;
    float frameTime;
    float accumulator;
    std::vector<sf::IntRect> idleFrames;
    std::vector<sf::IntRect> runFrames;
    std::vector<sf::IntRect> jumpUpFrames;
    std::vector<sf::IntRect> jumpDownFrames;
    std::vector<sf::IntRect> landingFrames;
    std::vector<sf::IntRect> atk1Frames;
    std::vector<sf::IntRect> atk2Frames;
};

struct HitboxProperties {
    float offsetX;
    float offsetY;
    float hitboxWidth;
    float hitboxHeight;
    sf::FloatRect hitbox;
};

class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    static const float gravity;
    bool isShooting = false;

    PhysicsProperties physics;
    AnimationProperties animation;
    HitboxProperties hitboxProps;

    std::vector<std::unique_ptr<Projectile>> projectiles;
    PlayerState currentState = PlayerState::Idle;
    sf::RenderWindow& window;
    GameWorld& gameWorld;


    void initializeSpriteProperties();
    void initializePhysicsProperties();
    void initializeHitboxProperties();
    void applyGravity(float dt);
    void handleMovementInput(float dt);
    void applyFriction(float dt);
    void moveVertically(float dt);
    void moveHorizontally(float dt);
    void updateAnimation(float dt);
    void spawnProjectile();
    void updateSpriteDirection();
    bool updateAnimationFrame(float dt, const std::vector<sf::IntRect>& frames, float currentFrameTime);
    void transitionToState(PlayerState newState);

    // State handler function declarations
    void handleIdleState(float dt);
    void handleRunningState(float dt);
    void handleJumpState(float dt);
    void handleJ_DownState(float dt);
    void handleLandingState(float dt);
    void handleAtk1State(float dt);
    void handleAtk2State(float dt);

public:
    Player(GameWorld& gw, sf::RenderWindow& win);
    void handleInput(float dt);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const;
    void updateProjectiles(float dt, GameWorld& gameWorld);
    sf::Vector2f mousePosition;
};

#endif
