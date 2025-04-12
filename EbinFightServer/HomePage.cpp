#include "HomePage.h"

HomePage::HomePage(Server& server, std::stack<State*>& currentState) : State(server, currentState) 
, m_title(Global::font)
{
    Init();
}

void HomePage::Init()
{
    m_mapName = "";
    m_title.setString("Home Page");
    m_title.setScale(sf::Vector2f(3, 3));
    m_title.setOrigin(m_title.getLocalBounds().getCenter());
    m_title.setPosition(sf::Vector2f(Global::win_width / 2, 100));

    m_mapButtons.clear();
    m_removeMapButtons.clear();


    float startY = 200;
    float spacing = 60;
    auto allTiles = p_server.GetMapJson().GetAllMapsData();
    if (allTiles.is_null()) {
        // Handle the error or log
        std::cerr << "Invalid JSON object!" << std::endl;
        return;
    }

    for (auto& [mapName, mapData] : allTiles.items())
    {
        Button* btn = new Button(Global::win_width / 2, startY, 250, 50, mapName);
        m_mapButtons.push_back(btn);

        Button* removeBtn = new Button(Global::win_width / 2 + 300, startY, 100, 50, "Remove", sf::Color::Red);
        m_removeMapButtons.push_back(removeBtn);  // Store the remove button in a separate container

        startY += spacing;
    }
    m_addMapButton = new Button(Global::win_width / 2, Global::win_height - 200, 250, 50, "Add Map", sf::Color::Green);
}


void HomePage::Handle_Events(const sf::Event& event, float dt)
{
   
        if (event.getIf<sf::Event::MouseButtonPressed>())
        {
            auto KeyPressed = event.getIf<sf::Event::MouseButtonPressed>();
            if (KeyPressed->button == sf::Mouse::Button::Left) {
                // Check if the button was clicked
                if (m_addMapButton && m_addMapButton->IsPressed(m_mousePos)) {

                    std::string name = "map"+std::to_string(p_server.GetMapJson().GetAllMapsData().size() + 1);
                    p_server.GetMapJson().AddMap(name);
                    m_mapName = name;

                    this->OnExitState();
                }
                for (int i = 0; i < m_mapButtons.size(); i ++)
                {
                    if (m_mapButtons[i] && m_mapButtons[i]->IsPressed(m_mousePos)) {
                        m_mapName = m_mapButtons[i]->GetLable();
                        this->OnExitState();
                    }
                    if (m_removeMapButtons[i] && m_removeMapButtons[i]->IsPressed(m_mousePos)) {
                        m_mapName = m_mapButtons[i]->GetLable();
                        p_server.GetMapJson().RemoveMap(m_mapName);
                        this->Init();
                    }
                }
               
            }
        }
    
}

void HomePage::Update(float dt)
{
   /* for (auto map : *m_sendMap)
    {

    }*/
}

void HomePage::Render(sf::RenderWindow& window)
{
    m_mousePos = sf::Mouse::getPosition(window);
    window.draw(m_title);
    for(auto mapButton : m_mapButtons)
        mapButton->Render(window);
    for (auto mapButton : m_removeMapButtons)
        mapButton->Render(window);

    m_addMapButton->Render(window);
}

void HomePage::OnExitState()
{
    p_currentState.push(new MapEditor(p_server, p_currentState, m_mapName));
}

