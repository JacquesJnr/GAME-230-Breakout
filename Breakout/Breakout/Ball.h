#pragma once
#include "Includes.h"

class Ball
{
public:

	Ball(sf::Texture* texture, sf::Vector2f size, sf::Vector2f Position);

	void Logic(sf::RenderWindow& MyWindow);

	void UpdateBallCollision();

	void MoveBall(sf::RenderWindow& MyWindow);

	void SetSpeed(int pspeed);

	void SetVelocity(std::string Command);

	sf::FloatRect GetBallCollision();

	sf::Sprite GetBallSprite();

private:

	sf::Sprite BallSprite;

	int velx;
	int vely;
	int speed;

	sf::FloatRect BallCollision;
};

