#include "Player.h"

unsigned Player::players = 0;

enum controls { UP = 0, DOWN, LEFT, RIGHT, SHOOT };

Player::Player(
	std::vector<Texture>& textures,
	int UP, int DOWN,
	int LEFT, int RIGHT,
	int SHOOT)
	:level(1), exp(0),
	hp(10), hpMax(10),
	statPoints(0),
	damage(1), damageMax(2),
	score(0)
{
	this->buffer.loadFromFile("Audio/hit.wav");

	//Dt
	this->dtMultiplier = 62.5f;

	//Stats
	this->expNext = 20 + static_cast<int>(
		(50 / 3)
		* ((pow(level, 3) - 6
			* pow(level, 2)) + 17
			* level - 12)
		);

	std::cout << this->expNext << "\n";


	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	this->sprite.setTexture(textures[0]);

	this->sprite.setScale(0.3f, 0.3f);

	this->bulletTexture = &textures[1];

	this->shootTimerMax = 25.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = 10.f;
	this->damageTimer = this->damageTimer;

	this->controls[controls::UP] = UP;
	this->controls[controls::DOWN] = DOWN;
	this->controls[controls::LEFT] = LEFT;
	this->controls[controls::RIGHT] = RIGHT;
	this->controls[controls::SHOOT] = SHOOT;

	this->maxVelocity = 25.f;
	this->acceleration = 0.8f;
	this->stabilizerForce = 0.4f;
}

Player::~Player()
{

}

int Player::getDamage()const
{
	return rand() % this->damageMax + this->damage;
}

bool Player::UpdateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->level++;
		this->statPoints++;
		this->exp -= this->expNext;
		this->expNext = static_cast<int>((50 / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));
		this->hpMax++;
		this->hp = hpMax;

		return true;
	}

	return false;
}

void Player::Movement(const float& dt)
{
	//UP
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP])))
	{
		this->direction.y = -1.f;

		if (this->currentVelocity.y > -this->maxVelocity && direction.y < 0)
			this->currentVelocity.y += this->direction.y * this->acceleration
			* dt * this->dtMultiplier;
	}

	//DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN])))
	{
		this->direction.x = 0.f;
		this->direction.y = 1.f;

		this->currentVelocity.y += this->direction.y * this->acceleration
			* dt * this->dtMultiplier;

	}

	//LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT])))
	{
		this->direction.x = -1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x > -this->maxVelocity && direction.x < 0)
			this->currentVelocity.x += this->direction.x * this->acceleration
			* dt * this->dtMultiplier;

	}

	//RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT])))
	{
		this->direction.x = 1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x < this->maxVelocity && direction.x > 0)
			this->currentVelocity.x += this->direction.x * this->acceleration
			* dt * this->dtMultiplier;

	}

	//Drag force
	if (this->currentVelocity.x > 0)
	{
		this->currentVelocity.x -= this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.x < 0)
			this->currentVelocity.x = 0;
	}

	else if (this->currentVelocity.x < 0)
	{
		this->currentVelocity.x += this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.x > 0)
			this->currentVelocity.x = 0;
	}

	if (this->currentVelocity.y > 0)
	{
		this->currentVelocity.y -= this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.y < 0)
			this->currentVelocity.y = 0;
	}

	else if (this->currentVelocity.y < 0)
	{
		this->currentVelocity.y += this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.y > 0)
			this->currentVelocity.y = 0;
	}


	//Final move
	this->sprite.move(this->currentVelocity.x * dt * this->dtMultiplier,
		this->currentVelocity.y * dt * this->dtMultiplier);

}

void Player::Combat(const float& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT]))
		&& this->shootTimer >= this->shootTimerMax)
	{
		this->sound.setBuffer(this->buffer);
		this->sound.setVolume(40);
		this->sound.play();
		this->bullets.push_back(
			Bullet(bulletTexture,
				Vector2f(this->playerCenter.x + 70.f, this->playerCenter.y),
				Vector2f(1.f, 0.f),
				2.f, 45.f, 1.f));

		this->shootTimer = 0; //RESET TIMER!
	}
}

void Player::Update(Vector2u windowBounds, const float& dt)
{
	//Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer += 1.f * dt * this->dtMultiplier;

	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer += 1.f * dt * this->dtMultiplier;

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	this->Movement(dt);
	this->Combat(dt);

}

void Player::Draw(RenderTarget& target)
{
	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}

	target.draw(this->sprite);

}