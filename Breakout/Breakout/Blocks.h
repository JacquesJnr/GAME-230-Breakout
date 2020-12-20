#pragma once
#include "Definitions.h"
#include "SFML/Graphics.hpp"
#include <iostream>

class Blocks
{
public: 

	Blocks(sf::Texture* image, sf::Vector2f size);
	~Blocks();

	sf::RectangleShape body;
	int hitsToBreak;
private:
	sf::Texture color;
};

