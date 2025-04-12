#pragma once

#include "Globals.h"
#include "HitBoxComponent.h"

class GameObject
{
public:
	GameObject(const std::string& texture_filePath, const sf::Vector2f& pos, const sf::Vector2f& scale);
	GameObject(const sf::Sprite& sprite);

	virtual void Update(float dt);

	virtual void Render(sf::RenderWindow& window, const sf::Vector2f& camera);


	void AddHitBoxComponent(const sf::Vector2f& offset, const sf::Vector2f& size);







	HitBoxComponent* GetHitBoxComponent();

	sf::Sprite* GetSprite();

private:
	void InitTexture(const std::string& texture_filePath);
	void Init(const sf::Vector2f& pos, const sf::Vector2f& scale);

protected:
	sf::Sprite* p_sprite;
	HitBoxComponent* p_hitBoxComponent;

private:
	sf::Texture* m_spriteTexure;

};

