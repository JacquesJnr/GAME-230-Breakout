#include "Ball.h"

//Constructor: Initializes all the values

Ball::Ball(sf::Texture* texture, sf::Vector2f size, sf::Vector2f Position)
{


	UpdateBallCollision();
}


//Logic: Updates the FloatRect used in Collision for the Ball, moves the Ball's Sprite

void Ball::Logic(sf::RenderWindow& MyWindow)
{
	UpdateBallCollision();

	MoveBall(MyWindow);
}


//UpdateBallCollision: Updates the FloatRect used in Collision Detection

void Ball::UpdateBallCollision()
{
	
}


//MoveBall: Moves the Ball using the forumula: D = s(v) when D = Displacement, s = Speed, and v = Velocity

void Ball::MoveBall(sf::RenderWindow& MyWindow)
{
	
}

void Ball::SetVelocity(std::string Command)
{
	if (Command == "Right" && velx > 0)
	{
		velx = -velx;
	}

	if (Command == "Left" && velx < 0)
	{
		velx = -velx;
	}

	if (Command == "Top" && vely < 0)
	{
		vely = -vely;
	}

	if (Command == "Bottom" && vely > 0)
	{
		vely = -vely;
	}
}

sf::FloatRect Ball::GetBallCollision()
{
	return BallCollision;
}


sf::Sprite Ball::GetBallSprite()
{
	return BallSprite;
}


void Ball::SetSpeed(int pspeed)
{
	speed = pspeed;
}