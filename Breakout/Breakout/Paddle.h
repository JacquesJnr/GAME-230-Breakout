#pragma once
#include "Definitions.h"

class Paddle
{
public:

	Paddle(sf::Texture* texture, float mX, float mY);

	sf::RectangleShape shape;
	sf::Vector2f velocity;
	void Update(float deltaTime, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getSize().x / 2.f; }
	float right() { return x() + shape.getSize().x / 2.f; }
	float top() { return y() - shape.getSize().y / 2.f; }
	float bottom() { return y() + shape.getSize().y / 2.f; }

private:
	float paddleWidth{ 200.f }, paddleHeight{ 30.f }, paddleVelocity{ 100.f };
	sf::Vector2f mousePos;
};

