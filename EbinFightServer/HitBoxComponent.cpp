#include "HitBoxComponent.h"

HitBoxComponent::HitBoxComponent(sf::Sprite& sprite, const sf::Vector2f& offset, const sf::Vector2f& size)
    : Component(sprite), m_offset(offset)
{

    m_hitbox.setPosition(sprite.getPosition() + m_offset);
    m_hitbox.setSize(size == sf::Vector2f(0.f, 0.f) ? sprite.getGlobalBounds().size : size);
    m_hitbox.setOrigin(m_hitbox.getLocalBounds().getCenter());
    m_hitbox.setFillColor(sf::Color(0, 0, 0, 0));  // Transparent fill
    m_hitbox.setOutlineThickness(1.f);
    m_hitbox.setOutlineColor(sf::Color::Green);
}

void HitBoxComponent::Update(float dt)
{
    m_hitbox.setPosition(p_sprite.getPosition() + m_offset);
}

void HitBoxComponent::Render(sf::RenderWindow& window)
{
    window.draw(m_hitbox);
}

const sf::FloatRect HitBoxComponent::GetGlobalBounds() const
{
    return m_hitbox.getGlobalBounds();
}

void HitBoxComponent::setPosition(const sf::Vector2f& pos)
{
    m_hitbox.setPosition(pos);
}

void HitBoxComponent::setOffset(const sf::Vector2f& newOffset)
{
    m_offset = newOffset;
}

bool HitBoxComponent::IsCollide(const sf::FloatRect& rect)
{
    auto intersection = m_hitbox.getGlobalBounds().findIntersection(rect);

    // If the intersection has a non-zero width and height, there is a collision
    return intersection.has_value();
}
