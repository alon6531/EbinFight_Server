#include "Engine.h"

Engine::Engine(Server& server) : m_server(server)
{
    this->Init();
}

Engine::~Engine()
{
    delete m_window;
}

void Engine::Run()
{
    if (m_window)
    {
        while (m_window->isOpen())
        {
            m_dt = m_clock.restart().asSeconds() * 100;

            this->Handle_Events();

            this->Update();

            this->Render();
        }
    }

    this->CloseWindow();
}


void Engine::Init()
{
    m_winWidth = 1920;
    m_winHeight = 1080;

    Global::win_width = m_winWidth;
    Global::win_height = m_winHeight;
    
    m_window = new sf::RenderWindow(
        sf::VideoMode({ m_winWidth , m_winHeight }),
        "Server",
        sf::Style::Close);

    m_dt = 0;

    m_state.push(new HomePage(m_server, m_state));
}


void Engine::Handle_Events()
{

    while (const std::optional event = m_window->pollEvent())
    {

        if (event->is<sf::Event::Closed>())
            this->CloseWindow();

        auto keyEvent = event->getIf<sf::Event::KeyPressed>();
        if (keyEvent)
        {
            if (keyEvent->code == sf::Keyboard::Key::Escape)
                this->CloseWindow();


        }


        if (m_state.top())
        {
            m_state.top()->Handle_Events(*event, m_dt);
        }
    }
}

void Engine::Update()
{

    if (m_state.top())
    {
        m_state.top()->Update(m_dt);
    }
}

void Engine::Render()
{
    m_window->clear(sf::Color::Black);

    if (m_state.top())
    {
        m_state.top()->Render(*m_window);
    }



    m_window->display();
}

void Engine::CloseWindow()
{
    std::cout << "Engine:Closing window..." << "\n";
    m_window->close();
    //m_server.Disconnect();
}
