#pragma once

#include "Globals.h"

class Component
{
public:
	Component(sf::Sprite& sprite);

	virtual void Update(float dt) = 0;

	virtual void Render(sf::RenderWindow& window) = 0;


protected:
	sf::Sprite& p_sprite;

private:


};

