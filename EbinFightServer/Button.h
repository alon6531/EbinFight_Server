#pragma once

#include "Globals.h"

class Button {
public:
    Button(float x, float y, float width, float height, const std::string& label, sf::Color color = sf::Color::Blue);
    Button(sf::RectangleShape& shape);

    void update(const sf::Vector2i& mousePosition, bool mousePressed);
    void Render(sf::RenderWindow& window);

    bool IsPressed(const sf::Vector2i& mousePosition);

    const sf::String& GetLable() const;
    sf::RectangleShape& GetRect();
private:
   
    sf::RectangleShape m_shape;
    sf::Text* m_text;
    bool m_isHovered;
    bool m_isClicked;
};


