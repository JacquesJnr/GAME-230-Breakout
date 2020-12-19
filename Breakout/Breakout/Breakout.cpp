#include "Includes.h"
using namespace sf;

const int HEIGHT = 720;
const int WIDTH = 1280;

int main()
{
    RenderWindow app(VideoMode(1280, 720), "Breakout!");
    app.setFramerateLimit(60);

    app.clear();
    app.display();

    return 0;
}
