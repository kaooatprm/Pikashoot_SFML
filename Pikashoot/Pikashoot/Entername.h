#ifndef ENTERNAME_H
#define ENTERNAME_H

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <utility>
#include <algorithm>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"

class Entername
{
private:
	sf::RenderWindow* window;
	sf::Font font;
	std::string playerName;
	sf::Text playerLabel;
	sf::Text player;
	sf::Texture backgoundTex;
	sf::Sprite backgoundSprite;

public:
	Entername(sf::RenderWindow* window);
	virtual ~Entername();

	inline std::string getPlayerName() { return this->playerName; }
	void enterName(std::vector<sf::Event> events);
	void render();
};

#endif // !ENTERNAME_H