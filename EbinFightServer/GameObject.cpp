#include "GameObject.h"

GameObject::GameObject(const std::string& texture_filePath, const sf::Vector2f& pos, const sf::Vector2f& scale)
{
	this->InitTexture(texture_filePath);

	this->Init(pos, scale);
}

GameObject::GameObject(const sf::Sprite& sprite) : p_sprite(new sf::Sprite(sprite))
{
	m_spriteTexure = new sf::Texture(sprite.getTexture());
}

void GameObject::InitTexture(const std::string& texture_filePath)
{
	m_spriteTexure = new sf::Texture();
	std::string path = "..\\Assets\\" + texture_filePath;
	if (!m_spriteTexure->loadFromFile(path))
	{
		std::cerr << "GameObject:ERROR::CANT_OPEN_FILE: " << texture_filePath << std::endl;
		m_spriteTexure = NULL;
	}
}

void GameObject::Init(const sf::Vector2f& pos, const sf::Vector2f& scale)
{
	if (m_spriteTexure)
		p_sprite = new sf::Sprite(*m_spriteTexure);
	else
		return;

	p_sprite->setPosition(pos);
	p_sprite->setScale(scale);

}

void GameObject::Update(float dt)
{
	if (p_hitBoxComponent)
		p_hitBoxComponent->Update(dt);
}

void GameObject::Render(sf::RenderWindow& window, const sf::Vector2f& camera)
{
	if (p_sprite)
	{
		// Save original position
		sf::Vector2f originalPos = p_sprite->getPosition();

		if (p_hitBoxComponent)
			p_hitBoxComponent->Render(window);

		// Apply camera offset
		//m_sprite->setPosition(originalPos - camera);
		//std::cout << camera.x << "\n";
		// Draw sprite
		window.draw(*p_sprite);

		// Restore original position
		p_sprite->setPosition(originalPos);
	}



}

void GameObject::AddHitBoxComponent(const sf::Vector2f& offset, const sf::Vector2f& size)
{
	if (p_sprite)
		p_hitBoxComponent = new HitBoxComponent(*p_sprite, offset, size);
}

HitBoxComponent* GameObject::GetHitBoxComponent()
{
	if(p_hitBoxComponent)
		return p_hitBoxComponent;
}

sf::Sprite* GameObject::GetSprite()
{
	if (p_sprite)
		return p_sprite;
}
