#pragma once
#include "Globals.h"
#include "Server.h"
#include "Button.h"
#include "ObjectsJson.h"

class State
{
public:
	State(Server& server, std::stack<State*>& currentState);

	virtual void Handle_Events(const sf::Event& event, float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;

	virtual void OnExitState() = 0;
protected:
	std::stack<State*>& p_currentState;
	Server& p_server;
	bool isExitState;

};

