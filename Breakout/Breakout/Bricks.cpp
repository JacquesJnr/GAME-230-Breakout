#include "Bricks.h"

Bricks::Bricks(sf::Texture* texture, float mX, float mY)
{
    shape.setPosition(mX, mY);
    shape.setSize({ blockWidth, blockHeight });
    shape.setTexture(texture);
    shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
}
