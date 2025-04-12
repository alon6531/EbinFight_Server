#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& label, sf::Color color) {
    m_shape.setSize(sf::Vector2f(width, height));
    m_shape.setOrigin(m_shape.getLocalBounds().getCenter());
    m_shape.setPosition(sf::Vector2f(x, y));
    m_shape.setFillColor(color);


 
    
    m_text = new sf::Text(Global::font);
    m_text->setString(label);
    m_text->setCharacterSize(24);
    m_text->setFillColor(sf::Color::White);
    m_text->setOrigin(m_text->getLocalBounds().getCenter());
    m_text->setPosition(sf::Vector2f(x, y));

    m_isHovered = false;
    m_isClicked = false;
}

Button::Button(sf::RectangleShape& shape) : m_shape(shape)
{
    m_text = NULL;
    m_isHovered = false;
    m_isClicked = false;
}

void Button::update(const sf::Vector2i& mousePosition, bool mousePressed) {
    // Check if mouse is hovering over the button
    if (m_shape.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
        m_shape.setFillColor(sf::Color::Cyan);  // Hover effect
        m_isHovered = true;
        if (mousePressed) {
            m_isClicked = true;  // Button clicked
        }
    }
    else {
        m_shape.setFillColor(sf::Color::Blue);
        m_isHovered = false;
    }
}

void Button::Render(sf::RenderWindow& window) {
    window.draw(m_shape);
    if(m_text)
        window.draw(*m_text);
}

bool Button::IsPressed(const sf::Vector2i& mousePosition) {
    // Check if mouse is over the button and if the mouse button is pressed
    if (m_shape.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
        return true; // Button is pressed
    }
    return false; // Button is not pressed
}

const sf::String& Button::GetLable() const
{
    return m_text->getString();
}

sf::RectangleShape& Button::GetRect()
{
    return m_shape;
}
