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

    if (!paddleTexture.loadFromFile(IMAGE_PATH "ball.png"))
        return EXIT_FAILURE;
#pragma endregion

    struct Bricks {
        sf::RectangleShape body;
        Vector2f size;
    };
   
    Bricks red, orange, yellow, green;

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Update
        

        window.clear();
        window.display();
    }

    return 0;
}
