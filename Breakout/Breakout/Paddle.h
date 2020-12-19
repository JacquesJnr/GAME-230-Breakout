#pragma once
#include "SFML/Graphics.hpp"
#include "Definitions.h"

class Paddle
{
public:

	Paddle(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position);

	sf::RectangleShape body;


	void Update(float deltaTime);
	void draw(sf::RenderWindow& window);
};

