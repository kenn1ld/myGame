#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile {
private:
	sf::RectangleShape shape;
	float speed;
	sf::Vector2f direction;
	sf::VertexArray directionLine;
	float initialVerticalSpeed;
	virtual void initializeAttributes() = 0;

protected:
	sf::RectangleShape& getShape() { return shape; }
	const sf::RectangleShape& getShape() const { return shape; }
	float getSpeed() const { return speed; }
	const sf::Vector2f& getDirection() const { return direction; }
	sf::VertexArray& getDirectionLine() { return directionLine; }
	sf::FloatRect hitbox;

	// Physics attributes
	struct Physics {
		float verticalVelocity = 0.0f;
		float drag = 0.01f;
		float gravity = 500.0f;
		float restitution = 0.5f;
		float terminalVelocity = 400.0f;
	} physics;

public:
	virtual ~Projectile() = default;
	Projectile(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow& window) const;
	virtual bool isInsideScreen(const sf::RenderWindow& window) const;
	const sf::FloatRect& getHitbox() const { return hitbox; }
	bool isStuck = false;
	sf::Clock stuckTime;
};

class GreenArrow : public Projectile {
public:
	GreenArrow(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition);
	void initializeAttributes() override;
	~GreenArrow() override = default;
};

class RedArrow : public Projectile {
public:
	RedArrow(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition);
	void initializeAttributes() override;
	~RedArrow() override = default;
};

#endif
