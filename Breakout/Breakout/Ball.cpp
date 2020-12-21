#include "Ball.h"

Ball::Ball(float mX, float mY)
{
    shape.setPosition(mX, mY);
    shape.setRadius(ballRadius);
    shape.setOrigin(ballRadius, ballRadius);
}

void Ball::Update(float deltaTime)
{
    shape.move(velocity);

    // Keep ball within screen Width
    if (left() < 0) velocity.x = ballVelocity;
    else if (right() > WIDTH)
        velocity.x = -ballVelocity;

    // Keep ball within screen Height
    if (top() < 0)
        velocity.y = ballVelocity;
    else if (bottom() > HEIGHT + 100)
        velocity.y = -ballVelocity;
}

void Ball::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}
