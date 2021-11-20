#define _CRT_SECURE_NO_WARNINGS
#include "Entername.h"
Entername::Entername(sf::RenderWindow* window)
{
	this->window = window;
	this->font.loadFromFile("Fonts/pokemon.ttf");
	this->playerLabel.setFont(this->font);
	this->playerLabel.setCharacterSize(20);
	this->playerLabel.setFillColor(sf::Color::White);
	this->playerLabel.setPosition((1078 / 2) - (310 / 2), 115);
	this->playerLabel.setLetterSpacing(1.5);
	this->playerLabel.setString("Enter Your Name");

	this->player.setFont(this->font);
	this->player.setCharacterSize(20);
	this->player.setFillColor(sf::Color::White);
	this->player.setPosition((1078 / 2), 170);
	this->player.setLetterSpacing(1.5);
	this->player.setString(this->playerName);

	this->backgoundTex.loadFromFile("Textures/score_bg.JPG");
	this->backgoundSprite.setTexture(this->backgoundTex);
}

Entername::~Entername()
{

}

void Entername::enterName(std::vector<sf::Event> events)
{
	for (size_t i = 0; i < events.size(); i++)
	{
		if (events[i].text.unicode != 32 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && events[i].text.unicode != 63)
		{
			if (events[i].text.unicode == 8 && this->playerName.length() > 0)
			{
				this->playerName.erase(playerName.length() - 1);
			}
			else if (events[i].text.unicode < 128 && playerName.length() < 10 && events[i].text.unicode != 8)
			{
				this->playerName += static_cast<char>(events[i].text.unicode);
			}
			this->player.setString(playerName);
			this->player.setOrigin(this->player.getLocalBounds().width / 2, this->player.getLocalBounds().height / 2);
		}
	}
}

void Entername::render()
{
	this->window->clear();
	this->window->draw(this->backgoundSprite);
	this->window->draw(this->playerLabel);
	this->window->draw(this->player);
	this->window->display();
}