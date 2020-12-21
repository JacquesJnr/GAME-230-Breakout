#include "Paddle.h"

Paddle::Paddle(sf::Texture* texture, float mX, float mY)
{
    shape.setPosition(mX, mY);
    shape.setSize({ this->paddleWidth, this->paddleHeight });
    shape.setTexture(texture);
    shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);

}

void Paddle::Update(float deltaTime, sf::RenderWindow& window)
{
    shape.move(velocity);
    mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

    // Keeps paddle inside WIDTH
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
        velocity.x = -paddleVelocity * deltaTime;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
        right() < WIDTH)
        velocity.x = paddleVelocity * deltaTime;
    // If the user isn't pressing anything, stop moving.
    else
        velocity.x = 0;

    if (mousePos.x < WIDTH &&
        mousePos.x > 0 &&
        mousePos.y > 0 &&
        mousePos.y < HEIGHT)
        shape.setPosition(mousePos.x, 670);

}

void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(shape);
}
