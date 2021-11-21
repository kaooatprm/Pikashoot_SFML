#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"

class Item
{
private:
	sf::Texture* itemTexture;
	sf::Sprite itemSprite;
	float timer;
	float timerMax;
	int type;

public:
	Item(sf::Texture* texture, float PosX, float PosY, float scale, int type);
	virtual ~Item();

	inline int itemType() { return this->type; }
	const sf::FloatRect getBounds() const;
	const bool deleteItem();

	void updateItem();
	void render(sf::RenderTarget* target);
	void Update();
	void Draw(sf::RenderTarget& target);
};

#endif // !ITEM_H