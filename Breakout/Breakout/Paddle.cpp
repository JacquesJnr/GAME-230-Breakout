#include "Paddle.h"

Paddle::Paddle(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position)
{
	body.setTexture(texture);
	body.setSize(size);
	body.setPosition(WIDTH /2, HEIGHT - 50);
}

void Paddle::Update(float deltaTime)
{

}

void Paddle::draw(sf::RenderWindow& window)
{
}
