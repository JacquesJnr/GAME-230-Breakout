#include "Ball.h"

Ball::Ball(float mX, float mY)
{
    shape.setPosition(mX, mY);
    shape.setRadius(ballRadius);
    shape.setOrigin(ballRadius, ballRadius);

    buffer.loadFromFile(AUDIO_PATH "wallHit.wav");
    wallSound.setBuffer(buffer);
    wallSound.setVolume(50);
   
    loss.loadFromFile(AUDIO_PATH "hurt.wav");
    lifeSound.setBuffer(loss);
    lifeSound.setVolume(50);
}

void Ball::Update(float deltaTime)
{
    shape.move(velocity);

    // Keep ball within screen Width
    if (left() < 0) 
    {
        velocity.x = ballVelocity;
        wallSound.play();
    }
    else if (right() > WIDTH)
    {
        velocity.x = -ballVelocity;
        wallSound.play();
    }    

    // Keep ball within screen Height
    if (top() < 0)
    {
        velocity.y = ballVelocity;
        wallSound.play();
    } 
    else if (bottom() > HEIGHT + 100) 
    {
        velocity.y = -ballVelocity;
        state = Aiming;
        lives -= 1;
        lifeSound.play();
    }
    else
    {
        state = Playing;
    }

    if (lives == 0) {
        state = OutOfLives;
    }
}

void Ball::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}
