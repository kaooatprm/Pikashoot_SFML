#pragma once
#include"Bullet.h"
#include "SFML/Audio.hpp"

class Player
{
private:
	float dtMultiplier;

	unsigned playerNr;

	Vector2f playerCenter;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;

	Sprite sprite;
	RectangleShape hitBox;

	std::vector<Bullet> bullets;
	Texture* bulletTexture;

	int controls[5];
	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;
	float stabilizerForce;

	int level;
	int exp;
	int expNext;
	int statPoints;


	int hp;
	int hpMax;

	int damage;
	int damageMax;

	int score;

	SoundBuffer buffer;
	Sound sound;

public:
	Player(std::vector<Texture>& textures,
		int UP = 22, int DOWN = 18,
		int LEFT = 0, int RIGHT = 3,
		int SHOOT = 57);
	virtual ~Player();

	//Accesors
	inline std::vector<Bullet>& getBullets() { return this->bullets; }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	int getDamage()const;
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const int& getHp()const { return this->hp; }
	inline const int& getHpMax()const { return this->hpMax; }
	inline void takeDamage(int damage) { this->hp -= damage; }
	inline bool isAlive()const { return this->hp > 0; }
	inline const int& getPlayerNr()const { return this->playerNr; }
	inline const int& getLevel()const { return this->level; }
	inline const int& getExp()const { return this->exp; }
	inline const int& getExpNext()const { return this->expNext; }
	inline bool gainExp(int exp)
	{
		this->exp += exp;
		return this->UpdateLeveling();
	}
	inline void gainScore(int score) { this->score += score; }
	inline const int getScore()const { return this->score; }

	//Functions
	bool UpdateLeveling();
	void Combat(const float& dt);
	void Movement(const float& dt);
	void Update(Vector2u windowBounds, const float& dt);
	void Draw(RenderTarget& target);
	void plusHp(int hp);
	void plusEXP(int exp);

	//Statics
	static unsigned players;
};