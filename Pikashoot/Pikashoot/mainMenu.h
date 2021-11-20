#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"


class mainMenu
{
private:
	sf::Texture blackGroundTexture;
	sf::Sprite blackGroundSprite;

public:
	mainMenu();
	virtual ~mainMenu();

	void update();
	void render(sf::RenderTarget& target);
};

#endif // !MAINMENU_H