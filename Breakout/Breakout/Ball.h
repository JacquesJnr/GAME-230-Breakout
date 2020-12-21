#pragma once
#include "Definitions.h"
#include "GameState.h"

class Ball
{
public:

	Ball(float mX, float mY);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

	sf::CircleShape shape;
	// 2D vector that stores the Ball's velocity.
	sf::Vector2f velocity{ ballVelocity, -ballVelocity };

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
	float ballRadius{ 10.f }, ballVelocity{ 10.f };

	int lives = 4;

	sf::SoundBuffer buffer, loss;
	sf::Sound wallSound;
	sf::Sound lifeSound;
	GameState state;
private:

};

