#include "Bricks.h"

Bricks::Bricks(float X, float Y)
{
    shape.setPosition(X, Y);
    shape.setSize({ brickWidth, brickHeight });
    //shape.setTexture(texture);
    shape.setOrigin(brickWidth / 2.f, brickHeight / 2.f);
}

void Bricks::Draw(sf::RenderWindow* window)
{
    window->draw(shape);
}
