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
	this->textures[player].loadFromFile("Textures/pikachu1.png");
	this->textures.push_back(Texture());
	this->textures[bullet].loadFromFile("Textures/bullet01.png");
	this->textures.push_back(Texture());
	this->textures[enemy01].loadFromFile("Textures/enemy01.png");
	
	//Init player
	this->players.push_back(Player(this->textures));

	this->playersAlive = this->players.size();

	//	Init Enemies
	Enemy e1(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1);

	this->enemiesSaved.push_back(Enemy(e1));

	this->enemySpawnTimerMax = 20;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

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
		tempText.setColor(Color::Green);
		tempText.setString(std::to_string(i));

		this->followPlayerTexts.push_back(Text(tempText));

		//Static Text Init
		tempText.setFont(font);
		tempText.setCharacterSize(14);
		tempText.setColor(Color::Green);
		tempText.setString("");

		this->staticPlayerTexts.push_back(Text(tempText));
	}

	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::Red);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("GAME OVER!");
	this->gameOverText.setPosition(this->window->getSize().x / 2 - 100.f, this->window->getSize().y / 2);
}

void Game::UpdateUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->followPlayerTexts[i].setPosition(this->players[i].getPosition().x , this->players[i].getPosition().y - 10.f);
		this->followPlayerTexts[i].setString(this->players[i].getHpAsString());
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{

	}
}

void Game::Update()
{
	if (this->playersAlive > 0)
	{
		//Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
			this->enemySpawnTimer++;

		//Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->enemies.push_back(Enemy(
				&this->textures[enemy01], this->window->getSize(),
				Vector2f(0.f, 0.f),
				Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
				0, rand() % 3 + 1, 2, 1));
	
			this->enemySpawnTimer = 0; //Reset timer
		}

	

		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				//UPDATE PLAYERS
				this->players[i].Update(this->window->getSize());

				//Bullets update
				for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
				{
					this->players[i].getBullets()[k].Update();

					//Enemy collision check
					for (size_t j = 0; j < this->enemies.size(); j++)
					{
						if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{
							this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);

							if (this->enemies[j].getHP() > 0)
								this->enemies[j].takeDamage(this->players[i].getDamage());

							if (this->enemies[j].getHP() <= 0)
								this->enemies.erase(this->enemies.begin() + j);

							return;	//RETURN!!!
						}
					}

					//Window bounds check
					if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
					{
						this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
						return;	//RETURN!!!
					}
				}
			}
		}

		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update();

			for (size_t k = 0; k < this->players.size(); k++)
			{
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
					{
						this->players[k].takeDamage(this->enemies[i].getDamage());

						if (!this->players[k].isAlive())
							this->playersAlive--;

						this->enemies.erase(this->enemies.begin() + i);
						return;//RETURN!!!
					}
				}
			}
			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
			{
				this->enemies.erase(this->enemies.begin() + i);
				return;	//RETURN!!!
			}
		}

	//UPDATE UI
	this->UpdateUI();
	}
}

void Game::DrawUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->window->draw(this->followPlayerTexts[i]);
		}
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->window->draw(this->staticPlayerTexts[i]);
		}
	}

	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}
}

void Game::Draw()
{
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->players[i].Draw(*this->window);
		}
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemyText.setPosition(this->enemies[i].getPosition());

		this->enemyText.setString(
			std::to_string(this->enemies[i].getHP()) +
			"/" +
			std::to_string(this->enemies[i].getHPMax()));

		this->enemies[i].Draw(*this->window);
		this->window->draw(this->enemyText);
	}

	this->DrawUI();

	this->window->display();
}

