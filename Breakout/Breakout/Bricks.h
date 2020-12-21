#pragma once
#include "Definitions.h"

class Bricks
{
public:
    sf::RectangleShape shape;

    // This boolean value will be used to check
    // whether a brick has been hit or not.
    bool Destroyed{ false };

    // Brick Constructor
    Bricks(sf::Texture* texture, float mX, float mY);

    float x() { return shape.getPosition().x; }
    float y() { return shape.getPosition().y; }
    float left() { return x() - shape.getSize().x / 2.f; }
    float right() { return x() + shape.getSize().x / 2.f; }
    float top() { return y() - shape.getSize().y / 2.f; }
    float bottom() { return y() + shape.getSize().y / 2.f; }

private:
    float blockWidth{ 60.f }, blockHeight{ 20.f };
    int countBlocksX{ 11 }, countBlocksY{ 4 };
};

