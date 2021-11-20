#pragma once

#include"Player.h"
#include"Enemy.h"
#include "SFML/Audio.hpp"

class Game
{
private:
	SoundBuffer enemybuffer;
	Sound enemydead;

	SoundBuffer collibuffer;
	Sound colli;

	//Game
	RenderWindow* window;
	float dtMultiplier;

	//Score
	unsigned score;
	unsigned scoreMultiplier;
	float multiplierTimerMax;
	float multiplierTimer;
	int multiplierAdder;
	int multiplierAdderMax;

	//UI
	//Text
	Font font;
	Text followPlayerText;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;
	Text scoreText;

	//Bars
	RectangleShape playerExpBar;

	//Texttags
	std::vector<TextTag> textTags;

	//Players
	std::vector<Player> players;
	int playersAlive;

	//Enemies
	std::vector<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	//WorldBG
	Texture worldBGtexture;
	Sprite worldBG;

	//Textures
	std::vector<Texture> textures;

	//Bullet Sound
	SoundBuffer hitBuffer;
	Sound hitSound;

public:
	Game(RenderWindow* window);
	virtual ~Game();

	//Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	//Setters


	//Functions
	void InitUI();
	void InitWorld();
	void UpdateUIPlayer(int index);
	void UpdateUIEnemy(int index);
	void Update(const float& dt);
	void RenderWorld();
	void DrawUI();
	void Draw();
};