#include "Includes.h"
using namespace sf;


int main()
{
    Clock clock;
    float deltaTime = 0.0f;
    Time currentTime;

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Breakout!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);

    window.clear();
    window.display();

    deltaTime = clock.getElapsedTime().asSeconds();
    clock.restart();

    return 0;
}
