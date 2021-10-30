#define SFML_NO_DEPRECATED_WARNINGS
#include "Game.h"

enum textures {player = 0, bullet, enemy01};

Game::Game(RenderWindow* window)
{
	this->window = window;
	this->window->setFramerateLimit(60);

	//Init fonts
	this->font.loadFromFile("Fonts/pokemon.ttf");

	//Init textures
	this->textures.push_back(Texture());
	this->textures[player].loadFromFile("Textures/pikachu.png");
	this->textures.push_back(Texture());
	this->textures[bullet].loadFromFile("Textures/bullet01.png");
	this->textures.push_back(Texture());
	this->textures[enemy01].loadFromFile("Textures/enemy01.png");
	//Init player
	this->players.push_back(Player(this->textures));

	this->enemies.push_back(Enemy(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1));

	this->enemies.push_back(Enemy(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1));

	this->enemies.push_back(Enemy(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1));

	this->enemies.push_back(Enemy(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1));


	this->InitUI();
}

Game::~Game()
{

}

void Game::InitUI()
{
	Text tempText;
	for (size_t i = 0; i < this->players.size(); i++)
	{
		//Follow Text Init
		tempText.setFont(font);
		tempText.setCharacterSize(14);
		tempText.setColor(Color::White);
		tempText.setString(std::to_string(i));

		this->followPlayerTexts.push_back(Text(tempText));

		//Static Text Init
		tempText.setFont(font);
		tempText.setCharacterSize(14);
		tempText.setColor(Color::White);
		tempText.setString("");

		this->staticPlayerTexts.push_back(Text(tempText));
	}
}

void Game::UpdateUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->followPlayerTexts[i].setPosition(this->players[i].getPosition().x , this->players[i].getPosition().y - 10.f);
		this->followPlayerTexts[i].setString(std::to_string(i) + "		-		" + this->players[i].getHpAsString());
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{

	}
}

void Game::Update()
{
	for (size_t i = 0; i < this->players.size(); i++)
	{
		bool enemyRemoved = false;
		bool bulletRemoved = false;

		//UPDATE PLAYERS
		this->players[i].Update(this->window->getSize());

		//Bullets update
		for (size_t k = 0; k < this->players[i].getBullets().size() && !bulletRemoved; k++)
		{
			this->players[i].getBullets()[k].Update();
			//Window bounds check
			if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
			{
				this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
				break; //BREAK!!!

			}

			//Enemy collision check
			for (size_t j = 0; j < this->enemies.size() && !enemyRemoved; j++)
			{

				if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
				{
					this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
					this->enemies.erase(this->enemies.begin() + j);
					
					enemyRemoved = true;
					bulletRemoved = true;
				}

				else if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
				{
					this->enemies.erase(this->enemies.begin() + i);
					enemyRemoved = true;
				}

			}

			//Window bounds check
			if (!bulletRemoved && this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
			{
				this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
				
				bulletRemoved = true;
			}
		}
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Update();
	}

	//UPDATE UI
	this->UpdateUI();
}

void Game::DrawUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->window->draw(this->followPlayerTexts[i]);
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		this->window->draw(this->staticPlayerTexts[i]);
	}
}

void Game::Draw()
{
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); i++)
	{
		this->players[i].Draw(*this->window);
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(*this->window);
	}

	this->DrawUI();

	this->window->display();
}

