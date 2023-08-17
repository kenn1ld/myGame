#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile {
private:  // Change the access specifier here
    sf::RectangleShape shape;
    float speed;
    sf::Vector2f direction;
    sf::VertexArray directionLine;

    virtual void initializeAttributes() = 0;

protected: // Provide protected accessors for derived classes
    sf::RectangleShape& getShape() { return shape; }
    const sf::RectangleShape& getShape() const { return shape; }
    float getSpeed() const { return speed; }
    const sf::Vector2f& getDirection() const { return direction; }
    sf::VertexArray& getDirectionLine() { return directionLine; }

public:
    virtual ~Projectile() = default;
    Projectile(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition);
    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window) const;
    virtual bool isInsideScreen(const sf::RenderWindow& window) const;
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
