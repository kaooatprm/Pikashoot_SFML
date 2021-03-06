#define SFML_NO_DEPRECATED_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#include "Game.h"

enum textures { player = 0, bullet, enemy01 };

Game::Game(sf::RenderWindow* window)
{
	//General Sound
	this->enemybuffer.loadFromFile("Audio/explode.wav");
	this->collibuffer.loadFromFile("Audio/colli.wav");
	this->levelupBuffer.loadFromFile("Audio/levelup.wav");

	this->window = window;
	this->window->setFramerateLimit(200);
	this->dtMultiplier = 62.5f;
	this->scoreMultiplier = 1;
	this->scores = 0;
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

	//Item
	this->itemBuffer.loadFromFile("Audio/item.wav");
	this->itemSound.setBuffer(this->itemBuffer);
	this->itemSound.setVolume(30);
	this->itemTexture[0].loadFromFile("Textures/health.png");
	this->itemTexture[1].loadFromFile("Textures/powerup.png");
	this->setScale[0] = 0.2f;
	this->setScale[1] = 0.4f;
	this->itemSpawnTimerMax = 8.f;

	//Init player
	this->players.push_back(Player(this->textures));
	this->playersAlive = this->players.size();

	//Init Enemies
	this->enemySpawnTimerMax = 35.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;


	this->InitUI();
	this->InitWorld();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updatePollEvents();

		if (this->players[0].getHp() > 0)

			this->dt = this->clock.restart().asSeconds();
		else
		{
			gameOver = true;
		}
		std::cout << scores << std::endl;
		this->Update(dt);
		this->Draw();
		
	}
}

void Game::updatePollEvents()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
			this->window->close();
		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
			this->window->close();
	}
}

void Game::InitWorld()
{
	this->worldBGtexture.loadFromFile("Textures/bg.jpg");
	this->worldBG.setTexture(this->worldBGtexture);
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
	this->followPlayerText.setColor(Color::Green);

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
	this->scoreText.setFillColor(Color::Yellow);
	this->scoreText.setCharacterSize(25);
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

		if (this->scores > 100 && this->scores <= 200)
		{
			this->enemySpawnTimerMax = 30.f;
		}

		else if (this->scores > 200 && this->scores <= 400)
		{
			this->enemySpawnTimerMax = 25.f;
		}

		else if (this->scores > 400 && this->scores <= 600)
		{
			this->enemySpawnTimerMax = 20.f;
		}

		else if (this->scores > 600 && this->scores <= 1000)
		{
			this->enemySpawnTimerMax = 18.f;
		}

		else if (this->scores > 1000)
		{
			this->enemySpawnTimerMax = 15.f;
		}

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

		//Item
		this->itemSpawnTimer += this->dt;
		if (this->itemSpawnTimer >= this->itemSpawnTimerMax)
		{
			this->itemSpawnTimer -= this->itemSpawnTimerMax;
			this->randomItem = (rand() % 2);
			this->items.push_back(new Item(&this->itemTexture[randomItem], this->window->getSize().x + 100, this->window->getSize().y - ((rand() % this->window->getSize().y - 100) + 50.f), this->setScale[randomItem], this->randomItem));
		}

		//Update Items
		for (int i = 0; i < items.size(); i++)
		{
			this->items[i]->updateItem();
			if (this->items[i]->getBounds().intersects(this->players[i].getGlobalBounds()))
			{
				this->itemSound.play();
				if (this->items[i]->itemType() == 0)
				{
					this->players[i].plusHp(rand() % 5);
					delete this->items[i];
					this->items.erase(this->items.begin() + i);
				}
				else if (this->items[i]->itemType() == 1)
				{
					this->players[i].plusEXP(rand() % 35);
					delete this->items[i];
					this->items.erase(this->items.begin() + i);

				}
			}
			else if (this->items[i]->deleteItem())
			{
				delete this->items[i];
				this->items.erase(this->items.begin() + i);
			}
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
								this->enemydead.setBuffer(this->enemybuffer);
								this->enemydead.setVolume(30);
								this->enemydead.play();

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
									this->levelupSound.setBuffer(this->levelupBuffer);
									this->levelupSound.setVolume(30);
									this->levelupSound.play();
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
			this->scores = 0;
			this->scores += players[i].getScore();
			this->scoreText.setString(
				"Score: " +
				std::to_string(this->scores) +
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
						this->colli.setBuffer(this->collibuffer);
						this->colli.setVolume(30);
						this->colli.play();
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

void Game::RenderWorld()
{
	this->window->draw(this->worldBG);
}

void Game::DrawUI()
{

}

void Game::Draw()
{
	this->window->clear();

	//Draw worldBG
	this->RenderWorld();

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

	//ITEM 
	for (size_t i = 0; i < this->items.size(); i++)
	{
		this->items[i]->updateItem();
		this->items[i]->Draw(*this->window);
	}

	//GAME OVER
	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}

	//Score Text
	this->window->draw(this->scoreText);

	this->window->display();
}