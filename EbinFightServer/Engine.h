#pragma once

#include "HomePage.h"

#pragma once

class Engine
{
public:
	Engine(Server& server);
	~Engine();


	void Run();

private:
	void Init();
	void Handle_Events();
	void Update();
	void Render();
	void CloseWindow();

private:
	Server& m_server;

	sf::RenderWindow* m_window;
	unsigned int m_winWidth;
	unsigned int m_winHeight;

	sf::Clock m_clock;
	float m_dt;// delta time

	std::stack<State*> m_state;
};

