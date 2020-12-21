#include "Framework.h"
#include "GameState.h"
#include "Definitions.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"
#include "GameElements.h"

GameState currentState;
void SetState(); // Allows me to debug and switch between states
int roundCount;
int points;

template<class T1, class T2>
bool isIntersecting(T1& thisShape, T2& otherShape)
{
    return thisShape.right() >= otherShape.left() && thisShape.left() <= otherShape.right() &&
        thisShape.bottom() >= otherShape.top() && thisShape.top() <= otherShape.bottom();
}

void CheckCollisions(Paddle& _paddle, Ball& _ball)
{
    float _ballVelocity = 10.f;

    // If there's no intersection, return
    if (!isIntersecting(_paddle, _ball)) return;
    paddleSound.play();

    _ball.velocity.y = -_ball.velocity.y;

    // Direct the balls direction depending on the position where it hit the paddle
    if (_ball.x() < _paddle.x())
        _ball.velocity.x = -_ballVelocity;
    else
        _ball.velocity.x = _ballVelocity;
}

void BrickCollisions(Bricks& _brick, Ball& _ball)
{
    float ballVelocity = 10.f;

    // If there's no intersection, return
    if (!isIntersecting(_brick, _ball)) return;

    // Otherwise, the brick has been hit!
    _brick.Destroyed = true;
    brickSound.play(); 
    points += 5;

    // Calculates when magnitude of the overlap between the ball and the brick
    float overlapLeft{ _ball.right() - _brick.left() };
    float overlapRight{ _brick.right() - _ball.left() };
    float overlapTop{ _ball.bottom() - _brick.top() };
    float overlapBottom{ _brick.bottom() - _ball.top() };

    // If the magnitude of the left overlap is smaller than the
    // right one we can safely assume the ball hit the brick
    // from the left
    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));

    // We can apply the same idea for top/bottom collisions
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    // Let's store the minimum overlaps for the X and Y axes
    float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
    float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

    // If the magnitude of the X overlap is less than the magnitude
    // of the Y overlap, we can safely assume the ball hit the brick
    // horizontally - otherwise, the ball hit the brick vertically

    // Then, upon our assumptions, we change either the X or Y velocity
    // of the ball, creating a "realistic" response for the collision
    if (abs(minOverlapX) < abs(minOverlapY))
        _ball.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
    else
        _ball.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
}

int main()
{
    Clock clock; // System Clock
    float deltaTime = 0.0f; 
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Breakout!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

 #pragma region Textures
    if (!redBlock.loadFromFile(IMAGE_PATH "red.png"))
        return EXIT_FAILURE;
    if (!orangeBlock.loadFromFile(IMAGE_PATH "orange.png"))
        return EXIT_FAILURE;
    if (!yellowBlock.loadFromFile(IMAGE_PATH "yellow.png"))
        return EXIT_FAILURE;
    if (!greenBlock.loadFromFile(IMAGE_PATH "green.png"))
        return EXIT_FAILURE;
    if (!menuOverlay.loadFromFile(IMAGE_PATH "MainScreen.png"))
        return EXIT_FAILURE;

    // Paddle Texture
    Texture paddleTexture;

    if (!paddleTexture.loadFromFile(IMAGE_PATH "paddle.png"))
        return EXIT_FAILURE;
    paddleTexture.setSmooth(false);

    // Ball Texture
    Texture ballTexture;
    if (!ballTexture.loadFromFile(IMAGE_PATH "ball.png"))
        return EXIT_FAILURE;
    ballTexture.setSmooth(false);

#pragma endregion

#pragma region SoundBuffers
    if (!brickHit.loadFromFile(AUDIO_PATH "brickHit.wav"))
        return EXIT_FAILURE;
    if (!countdown1.loadFromFile(AUDIO_PATH "countdown1.wav"))
        return EXIT_FAILURE;
    if (!countdown2.loadFromFile(AUDIO_PATH "countdown2.wav"))
        return EXIT_FAILURE;
    if (!gameOver.loadFromFile(AUDIO_PATH "gameOver.wav"))
        return EXIT_FAILURE;
    if (!loseLife.loadFromFile(AUDIO_PATH "hurt.wav"))
        return EXIT_FAILURE;
    if (!paddleHit.loadFromFile(AUDIO_PATH "paddleHit.wav"))
        return EXIT_FAILURE;
    if (!gainPoints.loadFromFile(AUDIO_PATH "points.wav"))
        return EXIT_FAILURE;


#pragma endregion

#pragma region Sounds
    brickSound.setBuffer(brickHit); brickSound.setVolume(50);
    wallSound.setBuffer(wallHit); wallSound.setVolume(50);
    paddleSound.setBuffer(paddleHit); wallSound.setVolume(50);
    lifeSound.setBuffer(loseLife); lifeSound.setVolume(50);
    pointSound.setBuffer(gainPoints); pointSound.setVolume(50);
    gameOverSound.setBuffer(gameOver); gameOverSound.setVolume(50);
    countdownSound.setBuffer(countdown1); countdownSound.setVolume(50);
    countdownSound2.setBuffer(countdown2); countdownSound2.setVolume(50);
#pragma endregion

#pragma region Fonts
    if (!Bungee.loadFromFile(FONT_PATH "Bungee.ttf"))
        return EXIT_FAILURE;

    if (!PressStart.loadFromFile(FONT_PATH "PressStart2P.ttf"))
        return EXIT_FAILURE;
#pragma endregion

#pragma region Text
    stateIndicator.setPosition(0, 10); stateIndicator.setFont(PressStart); stateIndicator.setFillColor(Color::White); stateIndicator.setCharacterSize(14); stateIndicator.setString("Current State: ");
    stateText.setPosition(stateIndicator.getPosition().x + 210, stateIndicator.getPosition().y);  stateText.setFont(PressStart); stateText.setFillColor(Color::Red); stateText.setCharacterSize(16);
    startText.setPosition(410, 500); startText.setFont(PressStart); startText.setFillColor(Color::White); startText.setCharacterSize(16); startText.setString("Press Return To Start Game");
#pragma endregion

#pragma region Sprites
    Sprite MainMenuOverlay;
    MainMenuOverlay.setTexture(menuOverlay);
    MainMenuOverlay.setPosition(0, 0);
#pragma endregion

    float brickCountX = 20.f;
    float brickCountY = 4.f;
    float brickWidth = 60.f;
    float brickHeight = 20.f;

    vector<Bricks> bricks;

    for (int x{ 0 }; x < brickCountX; x++)
        for (int y{ 0 }; y < brickCountY; y++)
            bricks.emplace_back(
                (x + 0.8f) * (brickWidth + 2), (y + 2) * (brickHeight + 20));

    // Create Paddle
    Paddle playerPaddle(&paddleTexture, WIDTH /2 , 670);

    // Create Ball
    Ball ball{ 0,0 };
    ball.shape.setTexture(&ballTexture);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {          
            SetState();
            deltaTime = clock.getElapsedTime().asSeconds();
            clock.restart();

            // State Machine
            switch (currentState) {
            case 0:
                // Menu
                stateText.setString("Menu");
                window.draw(MainMenuOverlay);
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    currentState = Aiming;

                    points = 0;

                    //Init bricks
                    for (int x{ 0 }; x < brickCountX; x++)
                        for (int y{ 0 }; y < brickCountY; y++)
                            bricks.emplace_back(
                                (x + 0.8f) * (brickWidth + 2), (y + 2) * (brickHeight + 20));

                }
                break;
            case 1:
                // Aim
                stateText.setString("Aiming");
                // Wait for player input to start playing
                ball.shape.setPosition(playerPaddle.shape.getPosition().x, playerPaddle.shape.getPosition().y - 40); // Sets the ball to follow the paddle
                if (Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left)) {
                    currentState = Playing;
                }
                break;
            case 2:
                // Play          
                stateText.setString("Playing");
                break;
            case 3:
                // Paused
                stateText.setString("Paused");
                break;
            case 4:
                // Round End
                stateText.setString("Round Ended");
                break;
            case 5:
                // Exit
                stateText.setString("Exiting");
                break;
            }

            // Close Window
            if (event.type == Event::Closed)
                window.close();          
        }

        //Update
        window.clear();
        playerPaddle.draw(window);
        ball.Draw(window);
        auto s = to_string(points);
        pointText.setPosition(900, 0); pointText.setFont(PressStart); pointText.setFillColor(Color::White); pointText.setCharacterSize(40); pointText.setString(s);
        window.draw(pointText);
      
       //Draw each 'brick' in bricks and set the texture of the brick based on its height
        for (auto& brick : bricks) {
            if (brick.shape.getPosition().y == 80)
                brick.shape.setTexture(&redBlock);

            if (brick.shape.getPosition().y == 120)
                brick.shape.setTexture(&orangeBlock);

            if (brick.shape.getPosition().y == 160)
                brick.shape.setTexture(&yellowBlock);

            if (brick.shape.getPosition().y ==200)
                brick.shape.setTexture(&greenBlock);

            brick.Draw(window);
        }

        if (currentState == Menu) {
            window.draw(MainMenuOverlay);
            window.draw(startText);
        }

        if (currentState == Aiming) {
            ball.shape.setPosition(playerPaddle.shape.getPosition().x, playerPaddle.shape.getPosition().y - 40); // Sets the ball to follow the paddle
            playerPaddle.Update(deltaTime, window);
        }

        if (currentState == Playing) {
            ball.Update(deltaTime);
            playerPaddle.Update(deltaTime, window);
            CheckCollisions(playerPaddle, ball);

            for (auto& brick : bricks) BrickCollisions(brick, ball);

            // And we use the "erase-remove idiom" to remove all `destroyed`
            // blocks from the block vector - using a cool C++11 lambda!
            bricks.erase(remove_if(begin(bricks), end(bricks),
                [](const Bricks& mBrick)
            {
                return mBrick.Destroyed;
            }),
                end(bricks));

            ball.Draw(window);
            playerPaddle.draw(window);
            for (auto& brick : bricks) window.draw(brick.shape);
        }

        window.draw(stateIndicator);
        window.draw(stateText);
        window.display();
    }
    return 0;
}

// Allows me to debug and switch between states
void SetState() {

    if (Keyboard::isKeyPressed(Keyboard::Num1))
        currentState = Menu;

    if (Keyboard::isKeyPressed(Keyboard::Num2))
        currentState = Aiming;

    if (Keyboard::isKeyPressed(Keyboard::Num3))
        currentState = Playing;

    if (Keyboard::isKeyPressed(Keyboard::Num4))
        currentState = Paused;

    if (Keyboard::isKeyPressed(Keyboard::Num5))
        currentState = RoundEnd;

    if (Keyboard::isKeyPressed(Keyboard::Num6))
        currentState = Exit;
}
