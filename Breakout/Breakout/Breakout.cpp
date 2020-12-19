#include "Includes.h"
using namespace sf;


int main()
{
    RenderWindow app(VideoMode(WIDTH, HEIGHT), "Breakout!");
    app.setFramerateLimit(60);

    app.clear();
    app.display();

    return 0;
}
