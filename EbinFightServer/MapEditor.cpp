#include "MapEditor.h"
#include "HomePage.h"

MapEditor::MapEditor(Server& server, std::stack<State*>& currentState, const sf::String& mapName)
	: State(server, currentState), m_mapName(mapName)
{
	/*auto m_mapName = p_server.GetMapJson()->GetTiles(mapName);
	std::cout << m_mapName.dump(4) << std::endl;*/
	this->Init();

	this->LoadMap();
}

void MapEditor::Init()
{
	m_view = sf::View(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(Global::win_width, Global::win_height)));
	if (!m_tileTexture.loadFromFile("..\\Assets\\Map\\tiles.png"))
		std::cout << "CANT_LOAD: " << "..\\Assets\\Map\\tiles.png" << "\n";
	m_isObject = false;

	// set init optianl
	sf::Sprite tileOptinal(m_tileTexture);
	tileOptinal.setScale(sf::Vector2f(objectsSale, objectsSale));
	m_currentTexturePos = sf::Vector2i(0, 0);
	sf::IntRect textureRect = sf::IntRect(m_currentTexturePos, sf::Vector2i(m_tileWidth, m_tileHeight));
	tileOptinal.setTextureRect(textureRect);
	tileOptinal.setOrigin(tileOptinal.getLocalBounds().getCenter());
	m_Option.push(tileOptinal);


	// set init tiles optianl lable
	while (m_currentTexturePos.y <= m_tileTexture.getSize().y)
	{
		if (m_currentTexturePos.x >= m_tileTexture.getSize().x)
		{
			m_currentTexturePos.x = 0;  // Reset to the beginning of the row
			m_currentTexturePos.y += m_tileHeight;  // Move to the next row
		}

		sf::RectangleShape tileOptinal;
		tileOptinal.setTexture(&m_tileTexture);
		tileOptinal.setSize(sf::Vector2f(50, 50));
		sf::IntRect textureRect = sf::IntRect(m_currentTexturePos, sf::Vector2i(m_tileWidth, m_tileHeight));
		tileOptinal.setTextureRect(textureRect);
		tileOptinal.setOrigin(tileOptinal.getLocalBounds().getCenter());
		tileOptinal.setOutlineColor(sf::Color::Red);
		tileOptinal.setOutlineThickness(3);
		tileOptinal.setPosition(sf::Vector2f(m_currentTexturePos.x * 2 + 50, m_currentTexturePos.y * 2 + 400));
		m_tileOptionLabel.push_back(tileOptinal);

		m_currentTexturePos.x += m_tileWidth;
	}

	if (!m_objectTexture.loadFromFile("..\\Assets\\Map\\objects.png"))
		std::cout << "CANT_LOAD: " << "..\\Assets\\Map\\objects.png" << "\n";

	// set init object optianl lable
	m_currentTexturePos = sf::Vector2i(0, 0);
	while (m_currentTexturePos.y <= m_objectTexture.getSize().y - m_tileHeight)
	{
		if (m_currentTexturePos.x >= m_objectTexture.getSize().x)
		{
			m_currentTexturePos.x = 0;  // Reset to the beginning of the row
			m_currentTexturePos.y += m_tileHeight;  // Move to the next row
		}

		sf::RectangleShape objectOptinal;
		objectOptinal.setTexture(&m_objectTexture);
		objectOptinal.setSize(sf::Vector2f(50, 50));
		sf::IntRect textureRect = sf::IntRect(m_currentTexturePos, sf::Vector2i(m_tileWidth, m_tileHeight));
		objectOptinal.setTextureRect(textureRect);
		objectOptinal.setOrigin(tileOptinal.getLocalBounds().getCenter());
		objectOptinal.setOutlineColor(sf::Color::Red);
		objectOptinal.setOutlineThickness(3);
		objectOptinal.setPosition(sf::Vector2f(m_currentTexturePos.x * 2 + 1300, m_currentTexturePos.y * 2 + 600));
		m_objectOptionLabel.push_back(objectOptinal);

		m_currentTexturePos.x += m_tileWidth;
	}


	//m_objectOption.push(objectOptionLabel);




	// init buttons
	m_saveDataButton = new Button(Global::win_width / 2, Global::win_height - 200, 120, 50, "Save");
	m_cancelButton = new Button(Global::win_width / 2 + 400, Global::win_height - 200, 120, 50, "Cancel");
}

void MapEditor::LoadMap()
{
	// load titles
	auto tiles = p_server.GetMapJson().GetTiles(m_mapName);
	if (tiles.is_null()) {
		// Handle the error or log
		std::cerr << "Invalid JSON object!" << std::endl;
		return;
	}
	for (auto tile : tiles)
	{

		sf::Sprite tileSprite(m_tileTexture);
		if (tile["texture_pos"].is_array())
		{
			sf::Vector2i texture_pos(
				tile["texture_pos"][0].get<int>(),
				tile["texture_pos"][1].get<int>());
			sf::IntRect textureRect = sf::IntRect(texture_pos, sf::Vector2i(m_tileWidth, m_tileHeight));
			tileSprite.setTextureRect(textureRect);
		}
		if (tile["position"].is_array())
		{

			sf::Vector2f position(
				tile["position"][0].get<float>(),
				tile["position"][1].get<float>()
			);

			tileSprite.setPosition(position);
		}
		if (tile["scale"].is_array())
		{
			sf::Vector2f scale(
				tile["scale"][0].get<float>(),
				tile["scale"][1].get<float>()
			);
			tileSprite.setScale(scale);
		}
		if (tile["origin"].is_array())
		{
			sf::Vector2f origin(
				tile["origin"][0].get<float>(),
				tile["origin"][1].get<float>()
			);
			tileSprite.setOrigin(origin);
		}

		m_tiles.push_back(tileSprite);
	}

	// load objects
	auto objects = p_server.GetMapJson().GetObjects(m_mapName);
	if (objects.is_null()) {
		// Handle the error or log
		std::cerr << "Invalid JSON object!" << std::endl;
		return;
	}
	for (auto object : objects)
	{
		sf::Sprite objectSprite(m_objectTexture);

		if (object["AnimationComponent"][2].is_array())
		{
			auto& frames = object["AnimationComponent"][2];
			
			sf::IntRect textureRect(
				sf::Vector2i(frames[0][0][0].get<int>(), frames[0][0][1].get<int>()),
				sf::Vector2i(frames[0][1][0].get<int>(), frames[0][1][1].get<int>()));
			
			objectSprite.setTextureRect(textureRect);
			
		}
		if (object["pos"].is_array())
		{
			sf::Vector2f position(
				object["pos"][0].get<float>(),
				object["pos"][1].get<float>()
			);
			objectSprite.setPosition(position);
		}
		if (object["scale"].is_array())
		{
			sf::Vector2f scale(
				object["scale"][0].get<float>(),
				object["scale"][1].get<float>()
			);

			objectSprite.setScale(sf::Vector2f(scale.x, scale.y));
		}
		if (object["origin"] == "center")
		{
			objectSprite.setOrigin(objectSprite.getLocalBounds().getCenter());
		}
		GameObject gameObject = GameObject(objectSprite);
		if (object["HitBoxComponent"].is_array())
		{
			bool isHitBox = object["HitBoxComponent"][0].get<bool>();
			if (isHitBox)
			{
				sf::Vector2f hitBoxPos(
					object["HitBoxComponent"][1][0].get<float>(),
					object["HitBoxComponent"][1][1].get<float>()
				);
				sf::Vector2f hitBoxSize(
					object["HitBoxComponent"][2][0].get<float>(),
					object["HitBoxComponent"][2][1].get<float>()
				);
				gameObject.AddHitBoxComponent(hitBoxPos, hitBoxSize);
			}


			//objectSprite.setSize(hitBoxSize);
		}



		m_object.push_back(gameObject);
	}

}

sf::Vector2f MapEditor::snapToIsometric(const sf::Vector2f& currentPos)
{
	float tileWidth = 3 * m_tileWidth;
	float tileHeight = 3 * m_tileHeight / 2;

	// Step 1: Convert screen (world) position to isometric grid coords
	float gridX = (currentPos.x / (tileWidth / 2) + currentPos.y / (tileHeight / 2)) / 2;
	float gridY = (currentPos.y / (tileHeight / 2) - currentPos.x / (tileWidth / 2)) / 2;

	// Step 2: Round to nearest tile
	int roundedX = std::round(gridX);
	int roundedY = std::round(gridY);

	// Step 3: Convert back to screen position
	float snappedX = (roundedX - roundedY) * (tileWidth / 2);
	float snappedY = (roundedX + roundedY) * (tileHeight / 2);

	return sf::Vector2f(snappedX, snappedY);
}



void MapEditor::Handle_Events(const sf::Event& event, float dt)
{

	auto KeyPressed = event.getIf<sf::Event::MouseButtonPressed>();
	if (KeyPressed)
	{
		if (KeyPressed->button == sf::Mouse::Button::Left) {
			this->OnMousePressedLeft();
		}


		if (KeyPressed->button == sf::Mouse::Button::Right) {

			this->OnMousePressedRight();
		}
		if (KeyPressed->button == sf::Mouse::Button::Middle) {
			m_isDragging = true;
		}
	}



	auto KeyReleased = event.getIf<sf::Event::MouseButtonReleased>();
	if (KeyReleased)
	{
		if (KeyReleased->button == sf::Mouse::Button::Middle) {
			m_isDragging = false;
		}
	}

	auto MouseMoved = event.getIf<sf::Event::MouseMoved>();
	if (MouseMoved) {
		if (m_isDragging) {
			sf::Vector2f delta = m_mousePos - sf::Vector2f(m_dragStartPos);
			m_view.move(sf::Vector2f(-delta.x, -delta.y));
			m_dragStartPos = sf::Vector2i(m_mousePos);
		}
		else {
			m_dragStartPos = sf::Vector2i(m_mousePos);
		}
	}



	auto WheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>();
	if (WheelScrolled) {
		this->OnWheelScroll(*WheelScrolled);
	}
}

void MapEditor::SaveData()
{
	p_server.GetMapJson().RemoveAllDataFromMap(m_mapName);

	// Save the updated data to the file
	for (auto tile : m_tiles)
	{
		//add tile to map
		json tileToAdd = {
			{"texture_pos" , {tile.getTextureRect().position.x, tile.getTextureRect().position.y}},
			{"position" , {tile.getPosition().x, tile.getPosition().y}},
			{"scale" , {tile.getScale().x, tile.getScale().y}},
			{"origin" , {tile.getOrigin().x, tile.getOrigin().y}}
		};
		p_server.GetMapJson().AddTile(m_mapName, tileToAdd);
	}
	for (int i = 0; i < m_object.size(); i++)
	{
		GameObject object = m_object[i];
		//add object to map
		json objectToAdd = {
			{"name" , "mapObj" + std::to_string(i)},
			{"texture" , "Map\\objects.png"},
			{"AnimationComponent",
			{
			true, 0, {
					{
						{object.GetSprite()->getTextureRect().position.x, object.GetSprite()->getTextureRect().position.y},
						{object.GetSprite()->getTextureRect().size.x, object.GetSprite()->getTextureRect().size.y}
					},
				}
			}},
			{"pos" , {object.GetSprite()->getPosition().x, object.GetSprite()->getPosition().y}},
			{"scale" , {object.GetSprite()->getScale().x, object.GetSprite()->getScale().y}},
			{"origin" , "center"},
			{"HitBoxComponent", {true,{0, 0}, {object.GetHitBoxComponent()->GetHitBox().getSize().x, object.GetHitBoxComponent()->GetHitBox().getSize().y}}}

		};
		p_server.GetMapJson().AddObject(m_mapName, objectToAdd);
	}

	this->OnExitState();
	return;
}

void MapEditor::OnMousePressedLeft()
{
	if (!m_isDragging)
	{
		// Check if m_saveDataButton button was clicked
		if (m_saveDataButton && m_saveDataButton->IsPressed(sf::Vector2i(m_mousePos))) {

			this->SaveData();
		}

		// Check if m_cancelButton button was clicked
		if (m_cancelButton && m_cancelButton->IsPressed(sf::Vector2i(m_mousePos))) {


			this->OnExitState();
			return;
		}

		// Check if m_objectOptionLabel button was clicked
		for (int i = 0; i < m_objectOptionLabel.size(); i++)
		{
			if (m_objectOptionLabel[i].IsPressed(sf::Vector2i(m_mousePos)))
			{

				sf::Sprite objectOptinal(m_objectTexture);
				objectOptinal.setTextureRect(m_objectOptionLabel[i].GetRect().getTextureRect());
				objectOptinal.setOrigin(objectOptinal.getLocalBounds().getCenter());
				objectOptinal.setScale(sf::Vector2f(objectsSale, objectsSale));
				m_Option.push(objectOptinal);
				m_isObject = true;
				return;
			}

		}

		// Check if m_tileOptionLabel button was clicked
		for (int i = 0; i < m_tileOptionLabel.size(); i++)
		{
			if (m_tileOptionLabel[i].IsPressed(sf::Vector2i(m_mousePos)))
			{
				sf::Sprite tileOptinal(m_tileTexture);
				tileOptinal.setTextureRect(m_tileOptionLabel[i].GetRect().getTextureRect());
				tileOptinal.setOrigin(tileOptinal.getLocalBounds().getCenter());
				tileOptinal.setScale(sf::Vector2f(objectsSale, objectsSale));
				m_Option.push(tileOptinal);
				m_isObject = false;
				return;
			}

		}

		// Check if the tile or object was clicked
		if (m_isObject)
		{
			//add rect to map
			sf::Sprite object = m_Option.top();
			GameObject gameObject(object);
			gameObject.AddHitBoxComponent(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
			m_object.push_back(gameObject);

		}
		else
		{
			m_tiles.push_back(m_Option.top());
		}

	}
}

void MapEditor::OnMousePressedRight()
{
	// remove tile or object
	if (m_isObject)
	{
		for (auto it = m_object.rbegin(); it != m_object.rend(); ++it) {
			if (it->GetSprite()->getGlobalBounds().contains(m_mousePosPixel)) {
				// כדי למחוק מתוך reverse_iterator צריך להמיר ל-regular iterator
				m_object.erase(std::next(it).base());
				break;  // עצור אחרי הראשון שמצאת (העליון ביותר)
			}
		}
	}
	else
	{
		for (auto it = m_tiles.rbegin(); it != m_tiles.rend(); ++it) {
			if (it->getGlobalBounds().contains(m_mousePosPixel)) {
				// כדי למחוק מתוך reverse_iterator צריך להמיר ל-regular iterator
				m_tiles.erase(std::next(it).base());
				break;  // עצור אחרי הראשון שמצאת (העליון ביותר)
			}
		}
	}
}

void MapEditor::OnWheelScroll(const sf::Event::MouseWheelScrolled& wheel_Scrolled)
{

	// Check if the control key is pressed while scrolling handle zoom
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
		if (wheel_Scrolled.delta > 0 && m_zoomLevel > m_minZoom) {
			m_zoomLevel /= m_zoomStep;
			m_view.zoom(1.0f / m_zoomStep);
		}
		else if (wheel_Scrolled.delta < 0 && m_zoomLevel < m_maxZoom) {
			m_zoomLevel *= m_zoomStep;
			m_view.zoom(m_zoomStep);
		}
		return;
	}

	// Handle texture scrolling
	if (!m_isObject)
	{
		if (wheel_Scrolled.delta > 0) {
			m_currentTexturePos.x += m_tileWidth;
			if (m_currentTexturePos.x >= m_tileTexture.getSize().x) {
				m_currentTexturePos.x = 0;
				m_currentTexturePos.y += m_tileHeight;
				if (m_currentTexturePos.y >= m_tileTexture.getSize().y) {
					m_currentTexturePos.y = 0;
				}
			}
		}
		else if (wheel_Scrolled.delta < 0) {
			m_currentTexturePos.x -= m_tileWidth;
			if (m_currentTexturePos.x < 0) {
				m_currentTexturePos.x = m_tileTexture.getSize().x - m_tileWidth;
				m_currentTexturePos.y -= m_tileHeight;
				if (m_currentTexturePos.y < 0) {
					m_currentTexturePos.y = m_tileTexture.getSize().y - m_tileHeight;
				}
			}
		}

		sf::IntRect rect(sf::Vector2i(m_currentTexturePos.x, m_currentTexturePos.y), sf::Vector2i(m_tileWidth, m_tileHeight));
		m_Option.top().setTextureRect(rect);
	}

}

void MapEditor::Update(float dt)
{
	for (auto object : m_object)
	{
		object.Update(dt);
	}
	m_Option.top().setPosition(snapToIsometric(sf::Vector2f(m_mousePosPixel)));
	for (int i = 0; i < m_tileOptionLabel.size(); i++)
	{
		if (m_tileOptionLabel[i].GetRect().getTextureRect() == m_Option.top().getTextureRect())
			m_tileOptionLabel[i].GetRect().setOutlineColor(sf::Color::Green);
		else
			m_tileOptionLabel[i].GetRect().setOutlineColor(sf::Color::Red);
	}

}

void MapEditor::Render(sf::RenderWindow& window)
{
	window.setView(m_view);  // Use the camera view

	m_mousePosPixel = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	m_mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

	for (auto tile : m_tiles)
		window.draw(tile);

	window.draw(m_Option.top());

	for (auto object : m_object)
		object.Render(window, sf::Vector2f(0, 0));

	window.setView(window.getDefaultView());

	for (auto tileOptionLable : m_tileOptionLabel)
		tileOptionLable.Render(window);

	for (auto objectOptionLable : m_objectOptionLabel)
		objectOptionLable.Render(window);


	m_saveDataButton->Render(window);
	m_cancelButton->Render(window);
}

void MapEditor::OnExitState()
{

	p_currentState.push(new HomePage(p_server, p_currentState));
}


