#ifndef BOTTON_H
#define BOTTON_H

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"

enum Botton_State { BTN_Idle = 0, BTN_Hover, BTN_Active };

class Botton
{
private:
	short unsigned bottonState;
	sf::RectangleShape bottonShape;
	sf::Font* font;
	sf::Text text;
	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;

public:
	Botton(float x, float y, float width, float height, sf::Font* font, std::string text, unsigned int text_size,
		sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
	virtual ~Botton();

	inline sf::FloatRect getGobalBounds() { return this->bottonShape.getGlobalBounds(); }
	const bool isPressed() const;
	void update(const sf::Vector2f mousePos);
	void render(sf::RenderTarget* target);
};

#endif // !BOTTON_H