#include "Includes.h"
using namespace sf;


int main()
{
    Clock clock;
    float deltaTime = 0.0f;
    Time currentTime;

    // Window dimensions & type
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Breakout!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60); 

    // Delta time
    deltaTime = clock.getElapsedTime().asSeconds();
    clock.restart();

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

    struct Bricks {
        sf::RectangleShape body;
        Vector2f size = Vector2f(100, 50);
    };
   
    Bricks red, orange, yellow, green;
    
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
    sf::RectangleShape paddle;
    paddle.setTexture(&paddleTexture);
    paddle.setSize(Vector2f(200, 50));
    paddle.setPosition(WIDTH / 2 - 150, 670);   

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Update
            Vector2f mousePos(Mouse::getPosition().x, Mouse::getPosition().y);
            paddle.setPosition(Vector2f(mousePos.x - 420, 670));

            // Close
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

        window.display();
    }

    return 0;
}
