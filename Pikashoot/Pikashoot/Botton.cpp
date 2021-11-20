#include "Botton.h"

Botton::Botton(float x, float y, float width, float height, sf::Font* font, std::string text, unsigned int text_size, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor)
{
	this->bottonShape.setSize(sf::Vector2f(width, height));
	this->bottonShape.setPosition(x, y);
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setCharacterSize(text_size);
	this->text.setPosition(this->bottonShape.getPosition().x + (this->bottonShape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->bottonShape.getPosition().y);
	this->bottonState = BTN_Idle;
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;
	this->bottonShape.setFillColor(this->idleColor);
}

Botton::~Botton()
{

}

const bool Botton::isPressed() const
{
	if (this->bottonState == BTN_Active)
	{
		return true;
	}
	return false;
}

void Botton::update(const sf::Vector2f mousePos)
{
	this->bottonState = BTN_Idle;
	if (this->bottonShape.getGlobalBounds().contains(mousePos))
	{
		this->bottonState = BTN_Hover;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->bottonState = BTN_Active;
		}
	}

	switch (this->bottonState)
	{
	case BTN_Idle:
		this->bottonShape.setFillColor(this->idleColor);
		this->bottonShape.setScale(1.f, 1.f);
		this->text.setScale(1.f, 1.f);
		this->text.setFillColor(sf::Color(255, 255, 255, 255));
		break;
	case BTN_Hover:
		this->bottonShape.setFillColor(this->hoverColor);
		this->bottonShape.setScale(1.3f, 1.3f);
		this->text.setScale(1.3f, 1.3f);
		this->text.setFillColor(sf::Color(180, 0, 0, 255));
		break;
	case BTN_Active:
		this->bottonShape.setFillColor(this->activeColor);
		this->text.setFillColor(sf::Color(255, 255, 255, 255));
		break;
	default:
		this->bottonShape.setFillColor(sf::Color::Red);
		break;
	}
}

void Botton::render(sf::RenderTarget* target)
{
	target->draw(this->bottonShape);
	target->draw(this->text);
}