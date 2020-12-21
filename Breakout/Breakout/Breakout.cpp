#include "Framework.h"
#include "GameState.h"
#include "Definitions.h"
#include "Paddle.h"
#include "Ball.h"
#include "GameElements.h"
using namespace sf;

GameState currentState;
void SetState(); // Allows me to debug and switch between states

template<class T1, class T2>
bool isIntersecting(T1& thisShape, T2& otherShape)
{
    return thisShape.right() >= otherShape.left() && thisShape.left() <= otherShape.right() &&
        thisShape.bottom() >= otherShape.top() && thisShape.top() <= otherShape.bottom();
}

void CheckCollisions(Paddle& _paddle, Ball& _ball)
{
    // If there's no intersection, return
    if (!isIntersecting(_paddle, _ball)) return;

    // Otherwise let's "push" the ball upwards
    _ball.velocity.y = -_ball.velocity.y;

    // Direct the balls direction depending on the position where it hit the paddle
    if (_ball.x() < _paddle.x())
        _ball.velocity.x = -_ball.velocity.y;
    else
        _ball.velocity.x = -_ball.velocity.y;;
}

int main()
{
    Clock clock; // System Clock
    float deltaTime = 0.0f; 
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Breakout!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    struct Bricks {
        RectangleShape body;
        Vector2f size = Vector2f(100, 50);
    };

    Bricks red, orange, yellow, green;

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
    if (!wallHit.loadFromFile(AUDIO_PATH "wallHit.wav"))
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

#pragma region Red Brick
    red.body.setOrigin(0.5f, 0.5f);
    red.body.setPosition(WIDTH / 2 - 100, 50);
    red.body.setTexture(&redBlock);
    red.body.setSize(red.size);
#pragma endregion

#pragma region Yellow Brick
    yellow.body.setOrigin(0.5f, 0.5f);
    yellow.body.setPosition(WIDTH / 2 - 100, 100);
    yellow.body.setTexture(&yellowBlock);
    yellow.body.setSize(red.size);
#pragma endregion

#pragma region Orange Brick
    orange.body.setOrigin(0.5f, 0.5f);
    orange.body.setPosition(WIDTH / 2 - 100, 150);
    orange.body.setTexture(&orangeBlock);
    orange.body.setSize(red.size);
#pragma endregion
    
#pragma region Green Brick
    green.body.setOrigin(0.5f, 0.5f);
    green.body.setPosition(WIDTH / 2 - 100, 200);
    green.body.setTexture(&greenBlock);
    green.body.setSize(red.size);
#pragma endregion

#pragma region Sprites
    Sprite MainMenuOverlay;
    MainMenuOverlay.setTexture(menuOverlay);
    MainMenuOverlay.setPosition(0, 0);
#pragma endregion

    // Create Paddle
    Paddle playerPaddle(&paddleTexture, WIDTH /2 , 670);

    // Create Ball
    Ball ball{ playerPaddle.shape.getPosition().x + 100, playerPaddle.shape.getPosition().y - 20 };
    ball.shape.setTexture(&ballTexture);

    while (window.isOpen())
    {
        sf::Event event;
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
            if (event.type == sf::Event::Closed)
                window.close();          
        }       

        //Update
        window.clear();
        playerPaddle.draw(window);
        window.draw(ball.shape);
       
        if (currentState == Menu) {
            window.draw(MainMenuOverlay);
            window.draw(startText);
        }

        if (currentState == Aiming) {
            playerPaddle.Update(deltaTime, window);
        }

        if (currentState == Playing) {
            ball.Update(deltaTime);
            playerPaddle.Update(deltaTime, window);
            CheckCollisions(playerPaddle, ball);
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
