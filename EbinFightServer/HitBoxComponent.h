#pragma once
#include "Component.h"
class HitBoxComponent :
    public Component
{
public:
    HitBoxComponent(sf::Sprite& sprite, const sf::Vector2f& offset, const sf::Vector2f& size);

    void Update(float dt) override;
    void Render(sf::RenderWindow& window) override;

    const sf::FloatRect GetGlobalBounds() const;
    void setPosition(const sf::Vector2f& pos);
    void setOffset(const sf::Vector2f& newOffset);

    bool IsCollide(const sf::FloatRect& rect);

    sf::RectangleShape& GetHitBox() { return m_hitbox; }
private:
    sf::RectangleShape m_hitbox;
    sf::Vector2f m_offset;
};


