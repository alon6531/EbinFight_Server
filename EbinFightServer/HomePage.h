#pragma once
#include "MapEditor.h"



class HomePage :
    public State
{
public:
	HomePage(Server& server, std::stack<State*>& currentState);

	void Handle_Events(const sf::Event& event, float dt) override;
	void Update(float dt) override;
	void Render(sf::RenderWindow& window) override;

	void OnExitState() override;

private:
	void Init();

private:
	sf::Text m_title;
	sf::Vector2i m_mousePos;
	Button* m_addMapButton;
	std::vector<Button*> m_mapButtons;
	std::vector<Button*> m_removeMapButtons;
	std::string m_mapName;

	
};

