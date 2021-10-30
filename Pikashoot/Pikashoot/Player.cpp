#include "Player.h"

unsigned Player::players = 0;

enum controls {UP = 0, DOWN, LEFT, RIGHT, SHOOT};

Player::Player(
	std::vector<Texture> &textures,
	int UP , int DOWN ,
	int LEFT , int RIGHT ,
	int SHOOT)
	:level(1), exp(0), expNext(100),
	hp(10), hpMax(10),
	damage(1), damageMax(2),
	score(0)
{
	this->sprite.setTexture(textures[0]);

	this->sprite.setScale(0.3f, 0.3f);

	this->bulletTexture = &textures[1];

	this->shootTimerMax = 25;
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = 10;
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

void Player::Movement()
{
	//UP
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP])))
	{
		this->direction.y = -1.f;

		if (this->currentVelocity.y > -this->maxVelocity && direction.y < 0)
			this->currentVelocity.y += this->direction.y * this->acceleration;
	}

	//DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN])))
	{
		this->direction.x = 0.f;
		this->direction.y = 1.f;

		if (this->currentVelocity.x < this->maxVelocity && direction.x > 0)
			this->currentVelocity.x += this->direction.x * this->acceleration;

		if (this->currentVelocity.y < this->maxVelocity && direction.y > 0)
			this->currentVelocity.y += this->direction.y * this->acceleration;

	}

	//LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT])))
	{
		this->direction.x = -1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x > -this->maxVelocity && direction.x < 0)
			this->currentVelocity.x += this->direction.x * this->acceleration;

	}

	//RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT])))
	{
		this->direction.x = 1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x < this->maxVelocity && direction.x > 0)
			this->currentVelocity.x += this->direction.x * this->acceleration;

	}
	
	//Drag force
	if(this->currentVelocity.x > 0)
	{
		this->currentVelocity.x -= this->stabilizerForce;

		if (this->currentVelocity.x < 0)
			this->currentVelocity.x = 0;
	}

	else if (this->currentVelocity.x < 0)
	{
		this->currentVelocity.x += this->stabilizerForce;

		if (this->currentVelocity.x > 0)
			this->currentVelocity.x = 0;
	}

	if (this->currentVelocity.y > 0)
	{
		this->currentVelocity.y -= this->stabilizerForce;

		if (this->currentVelocity.y < 0)
			this->currentVelocity.y = 0;
	}

	else if (this->currentVelocity.y < 0)
	{
		this->currentVelocity.y += this->stabilizerForce;

		if (this->currentVelocity.y > 0)
			this->currentVelocity.y = 0;
	}
		

	//Final move
	this->sprite.move(this->currentVelocity.x , this->currentVelocity.y);
}

void Player::Combat()
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT]))
		&& this->shootTimer >= this->shootTimerMax)
	{
		this->bullets.push_back(
		Bullet(bulletTexture, 
			Vector2f(this->playerCenter.x + 70.f , this->playerCenter.y),
			Vector2f(1.f, 0.f), 2.f, 
			45.f, 1.f));
		
		this->shootTimer = 0; //RESET TIMER!
	}
}

void Player::Update(Vector2u windowBounds)
{
	//Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer++;

	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer++;

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x + 
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y + 
		this->sprite.getGlobalBounds().height / 2;

	this->Movement();
	this->Combat();
	
}

void Player::Draw(RenderTarget &target)
{
	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}

	target.draw(this->sprite);

}