#include "Includes.h"
using namespace sf;

GameState currentState;
void SetState();

int main()
{
    Clock clock;
    float deltaTime = 0.0f;
    Time currentTime;

    // Window dimensions & type
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Breakout!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);

    struct Bricks {
        RectangleShape body;
        Vector2f size = Vector2f(100, 50);
    };

    Bricks red, orange, yellow, green;

    struct Ball
    {
        RectangleShape body;
        Vector2f size = Vector2f(60, 60);
        float speed = 100;
    };

    Ball ball;

#pragma region Textures
    // Block Textures
    Texture redBlock, orangeBlock, yellowBlock, greenBlock;

    if (!redBlock.loadFromFile(IMAGE_PATH "red.png"))
        return EXIT_FAILURE;
    if (!orangeBlock.loadFromFile(IMAGE_PATH "orange.png"))
        return EXIT_FAILURE;
    if (!yellowBlock.loadFromFile(IMAGE_PATH "yellow.png"))
        return EXIT_FAILURE;
    if (!greenBlock.loadFromFile(IMAGE_PATH "green.png"))
        return EXIT_FAILURE;

    // Paddle Texture
    Texture paddleTexture;

    if (!paddleTexture.loadFromFile(IMAGE_PATH "paddle.png"))
        return EXIT_FAILURE;

    // Ball Texture
    Texture ballTexture;

    if (!ballTexture.loadFromFile(IMAGE_PATH "ball.png"))
        return EXIT_FAILURE;
#pragma endregion   

#pragma region Fonts
    Font Bungee, PressStart;

    if (!Bungee.loadFromFile(FONT_PATH "Bungee.ttf"))
        return EXIT_FAILURE;

    if (!PressStart.loadFromFile(FONT_PATH "PressStart2P.ttf"))
        return EXIT_FAILURE;
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

    RectangleShape paddle;
    paddle.setTexture(&paddleTexture);
    paddle.setSize(Vector2f(200, 50));
    paddle.setPosition(WIDTH / 2 - 150, 670);   

    ball.body.setTexture(&ballTexture);
    ball.body.setSize(ball.size);

    Text stateIndicator;
    stateIndicator.setPosition(0, 10); stateIndicator.setFont(PressStart); stateIndicator.setFillColor(Color::White); stateIndicator.setCharacterSize(14); stateIndicator.setString("Current State: ");

    Text stateText;
    stateText.setPosition(stateIndicator.getPosition().x + 210, stateIndicator.getPosition().y);  stateText.setFont(PressStart); stateText.setFillColor(Color::Red); stateText.setCharacterSize(16);

    currentState = Menu;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            Vector2f mousePos(Mouse::getPosition().x, Mouse::getPosition().y); // TODO add play on click or key press
            SetState();
            // Delta time
            deltaTime = clock.getElapsedTime().asSeconds();
            clock.restart();

            switch (currentState) {
            case 0:
                // Menu
                stateText.setString("Menu");
                break;
            case 1:
                // Play
                stateText.setString("Playing");
                paddle.setPosition(Vector2f(mousePos.x - 420, 670)); // Paddle follows mouse position
                ball.body.setPosition(paddle.getPosition().x + 65, paddle.getPosition().y - 100); // Sets the ball to follow the paddle
                break;
            case 2:
                // Paused
                stateText.setString("Paused");

                break;
            case 3:
                // Round End
                stateText.setString("Round Ended");
                break;
            case 4:
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
        window.draw(red.body);
        window.draw(yellow.body);
        window.draw(orange.body);
        window.draw(green.body);
        window.draw(paddle);
        window.draw(ball.body);
        window.draw(stateIndicator);
        window.draw(stateText);

        window.display();
    }

    return 0;
}

void SetState() {

    if (Keyboard::isKeyPressed(Keyboard::Num1))
        currentState = Menu;

    if (Keyboard::isKeyPressed(Keyboard::Num2))
        currentState = Play;

    if (Keyboard::isKeyPressed(Keyboard::Num3))
        currentState = Paused;

    if (Keyboard::isKeyPressed(Keyboard::Num4))
        currentState = RoundEnd;

    if (Keyboard::isKeyPressed(Keyboard::Num5))
        currentState = Exit;
}
