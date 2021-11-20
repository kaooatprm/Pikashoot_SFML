#ifndef HIGHSCORE_H
#define HIGHSCORE_H

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

class Highscore
{
private:
	sf::RenderWindow* window;
	float windowX = 1920;
	sf::Texture backgroundTexture;
	sf::Sprite background;

	FILE* high;
	sf::Font font;
	sf::Text scoreBoardText;

	sf::Text num[5];
	sf::Text playerName[5];
	sf::Text playerScore[5];

	char temp[255];
	int score[6];
	std::string name[6];

	std::vector <std::pair<int, std::string>> userScore;

	void initHighscore();
	void initBackground();

public:
	Highscore(sf::RenderWindow* window);
	virtual ~Highscore();
	void render();
};

#endif // !HIGHSCORE_H