#pragma once
#include "State.h"
#include "GameObject.h"

class MapEditor :
    public State
{
public:
	MapEditor(Server& server, std::stack<State*>& currentState, const sf::String& mapName);

	void Handle_Events(const sf::Event& event, float dt) override;
	void Update(float dt) override;
	void Render(sf::RenderWindow& window) override;

	void OnExitState() override;

private:
	void Init();
	void LoadMap();
	sf::Vector2f snapToIsometric(const sf::Vector2f& currentPos);

	void SaveData(); 
	void OnMousePressedLeft();
	void OnMousePressedRight();
	void OnWheelScroll(const sf::Event::MouseWheelScrolled& wheel_Scrolled);
private:
	sf::String m_mapName;
	sf::View m_view;
	float m_zoomLevel = 1.0f;
	const float m_zoomStep = 1.1f;
	const float m_minZoom = 0.5f;
	const float m_maxZoom = 3.0f;
	sf::Vector2i m_dragStartPos;  // Start position when dragging
	bool m_isDragging = false;
	sf::Vector2f m_mousePosPixel;
	sf::Vector2f m_mousePos;

	bool m_isObject;

	std::stack<sf::Sprite> m_Option;
	const float objectsSale = 3;

	std::vector<Button> m_tileOptionLabel;
	sf::Vector2i m_currentTexturePos;
	sf::Texture m_tileTexture;
	std::vector<sf::Sprite> m_tiles;
	const float m_tileWidth = 32;
	const float m_tileHeight = 32;


	//std::stack<sf::RectangleShape> m_objectOption;
	sf::Texture m_objectTexture;
	std::vector<Button> m_objectOptionLabel;
	std::vector<GameObject> m_object;



	Button* m_saveDataButton;
	Button* m_cancelButton;



};

