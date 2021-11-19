#define SFML_NO_DEPRECATED_WARNINGS
#include "Game.h"

enum textures { player = 0, bullet, enemy01 };

Game::Game(RenderWindow* window)
{
	this->window = window;
	this->window->setFramerateLimit(200);
	this->dtMultiplier = 62.5f;
	this->scoreMultiplier = 1;
	this->score = 0;
	this->multiplierAdderMax = 10;
	this->multiplierAdder = 0;
	this->multiplierTimerMax = 200.f;
	this->multiplierTimer = this->multiplierTimerMax;

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
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	this->InitUI();
}

Game::~Game()
{

}

void Game::InitUI()
{
	Text tempText;

	//Follow Text Init
	this->followPlayerText.setFont(font);
	this->followPlayerText.setCharacterSize(14);
	this->followPlayerText.setColor(Color::White);

	//Static Text Init
	this->staticPlayerText.setFont(font);
	this->staticPlayerText.setCharacterSize(14);
	this->staticPlayerText.setColor(Color::White);

	this->playerExpBar.setSize(Vector2f(90.f, 10.f));
	this->playerExpBar.setFillColor(Color(0.f, 90.f, 200.f, 200.f));


	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::Red);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("GAME OVER!");
	this->gameOverText.setPosition(this->window->getSize().x / 2 - 100.f, this->window->getSize().y / 2);

	this->scoreText.setFont(this->font);
	this->scoreText.setFillColor(Color(200, 200, 200, 150));
	this->scoreText.setCharacterSize(20);
	this->scoreText.setString("Score: 0");
	this->scoreText.setPosition(10.f, 10.f);
}

void Game::UpdateUIPlayer(int index)
{
	if (index >= this->players.size())
		std::cout << "OUT OF BOUNDS! (UPDATEUI)";
	else //FOLLOW TEXT
	{
		this->followPlayerText.setPosition(
			this->players[index].getPosition().x,
			this->players[index].getPosition().y - 22.f
		);

		this->followPlayerText.setString(
			//std::to_string(this->players[index].getPlayerNr())
			+""
			+ this->players[index].getHpAsString()
			+ "\n\n\n\n\n\n\n\n"
			+ std::to_string(this->players[index].getLevel())
		);

		//BARS
		this->playerExpBar.setPosition(
			this->players[index].getPosition().x + 25.f,
			this->players[index].getPosition().y + 150.f
		);

		this->playerExpBar.setScale(
			(static_cast<float>(this->players[index].getExp()) / this->players[index].getExpNext()),
			1.f
		);

		//STATIC TEXT
	}
}

void Game::UpdateUIEnemy(int index)
{
	this->enemyText.setPosition(this->enemies[index].getPosition());

	this->enemyText.setString(
		std::to_string(this->enemies[index].getHP()) +
		"/" +
		std::to_string(this->enemies[index].getHPMax()));
}

void Game::Update(const float& dt)
{
	if (this->playersAlive > 0)
	{
		//Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
			this->enemySpawnTimer += 1.f * dt * this->dtMultiplier;

		//Score timer and multipliers
		if (this->multiplierTimer > 0.f)
		{
			this->multiplierTimer -= 1.f * dt * this->dtMultiplier;

			if (this->multiplierTimer <= 0.f)
			{
				this->multiplierTimer = 0.f;
				this->multiplierAdder = 0;
				this->scoreMultiplier = 1;
			}
		}

		if (this->multiplierAdder >= this->multiplierAdderMax)
		{
			this->multiplierAdder = 0;
			this->scoreMultiplier++;
		}

		//Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->enemies.push_back(Enemy(
				&this->textures[enemy01], this->window->getSize(),
				Vector2f(0.f, 0.f),
				Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
				rand() % 2, rand() % 3 + 1, 2, 1, rand() % this->playersAlive)
			);

			this->enemySpawnTimer = 0; //Reset timer
		}


		//Update players, bullets and combat
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				//UPDATE PLAYERS
				this->players[i].Update(this->window->getSize(), dt);

				//Bullets update
				for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
				{
					this->players[i].getBullets()[k].Update(dt);

					//Enemy collision check
					for (size_t j = 0; j < this->enemies.size(); j++)
					{
						if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{
							this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);

							//Enemy take damage
							int damage = this->players[i].getDamage();
							if (this->enemies[j].getHP() > 0)
								this->enemies[j].takeDamage(this->players[i].getDamage());

							//Create text tag
							this->textTags.push_back(
								TextTag(&this->font,
									"-" + std::to_string(damage),
									Color::Red,
									Vector2f(this->enemies[j].getPosition().x + 20.f,
										this->enemies[j].getPosition().y - 55.f),
									20,
									15.f
								)
							);
							//Enemy dead
							if (this->enemies[j].getHP() <= 0)
							{
								//GAIN EXP
								int exp = this->enemies[j].getHPMax()
									+ (rand() % this->enemies[j].getHPMax() + 1);

								//GAIN SCORE & RESET MULTIPLIER TIMER
								this->multiplierTimer = this->multiplierTimerMax;
								int score = this->enemies[j].getHPMax() * this->scoreMultiplier;
								this->multiplierAdder++;
								this->players[i].gainScore(score);

								
								if (this->players[i].gainExp(exp))
								{
								//LEVEL UP TEXT TAG
								this->textTags.push_back(
									TextTag(&this->font,
										"LEVEL UP!",
										Color::Cyan,
										Vector2f(this->players[i].getPosition().x + 50.f,
											this->players[i].getPosition().y + 55.f),
										20,
										15.f
									)
								);
								}

								this->enemies.erase(this->enemies.begin() + j);

								//GAIN EXP TEXT TAG
								this->textTags.push_back(
									TextTag(&this->font,
										"+" + std::to_string(exp) + " exp",
										Color::Cyan,
										Vector2f(this->players[i].getPosition().x + 50.f,
											this->players[i].getPosition().y - 55.f),
										20,
										15.f
									)
								);
							}

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
			//UPDATE SCORE
			this->score = 0;
			this->score += players[i].getScore();
			this->scoreText.setString(
				"Score: " +
				std::to_string(this->score) +
				"\nMultiplier:" +
				std::to_string(this->scoreMultiplier) +
				"\nMultiplier Timer:" +
				std::to_string((int)this->multiplierTimer) +
				"\nMultiplier Stack: " +
				std::to_string(this->multiplierAdder) +
				" / " +
				std::to_string(this->multiplierAdderMax)
			);
		}


		//Update enemies
		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update(dt, this->players[this->enemies[i].getPlayerFollowNr()].getPosition());

			//Enemy player collision
			for (size_t k = 0; k < this->players.size(); k++)
			{
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
					{
						int damage = this->enemies[i].getDamage();
							this->players[k].takeDamage(damage);
							//Create text tag
							this->textTags.push_back(
								TextTag(&this->font, 
									"-" + std::to_string(damage),
									Color::Red, 
									Vector2f(this->players[k].getPosition().x + 20.f,
										this->players[k].getPosition().y - 55.f),
									20,
									15.f
								)
							);

						if (!this->players[k].isAlive())
							this->playersAlive--;

						this->enemies.erase(this->enemies.begin() + i);
						return;//RETURN!!!
					}
				}
			}

			//Enemies out of bounds
			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
			{
				this->enemies.erase(this->enemies.begin() + i);
				return;	//RETURN!!!
			}
		}

		//Texttags
		for (size_t i = 0; i < this->textTags.size(); i++)
		{
			this->textTags[i].Update(dt);

			if (this->textTags[i].getTimer() <= 0.f)
			{
				this->textTags.erase(this->textTags.begin() + i);
				break;
			}
		}
	}
}

void Game::DrawUI()
{

}

void Game::Draw()
{
	this->window->clear();

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(*this->window);

		//UI
		this->UpdateUIEnemy(i);
		this->window->draw(this->enemyText);
	}


	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->players[i].Draw(*this->window);

			//UI
			this->UpdateUIPlayer(i);
			this->window->draw(this->followPlayerText);
			this->window->draw(this->playerExpBar);
		}
	}

	for (size_t i = 0; i < this->textTags.size(); i++)
	{
		this->textTags[i].Draw(*this->window);
	}

	//GAME OVER TEXT
	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}
	
	//Score Text
	this->window->draw(this->scoreText);

	this->window->display();
}