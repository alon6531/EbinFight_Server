#include "Server.h"
#include "Engine.h"

Server::Server(unsigned short port, const std::string& map_name) : m_port(port), m_mapName(map_name)
{
   
}

void Server::Start()
{
    m_mapJson = MapJson();
	m_playersJson = PlayersJson();

    std::thread autoSaveThread(&Server::AutoSaveLoop, this);
    

	//Listener();
    std::thread listenerThread(&Server::Listener, this);

    Engine app = Engine(*this);
	app.Run();
   
    listenerThread.join();

    autoSaveThread.join();
}

void Server::Listener()
{
	// Create TCP listener
    if (m_tcpListener.listen(m_port) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to bind listener to port " << m_port << "\n";
        return;
    }
    m_selector.add(m_tcpListener);
    std::cout << "Server is listening on port " << m_port << "\n";



	// Create UDP socket
    if (m_udpSocket.bind(m_port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to bind UDP socket to port " << m_port << "\n";
        return;
    }
    else {
        std::cout << "UDP socket bound to port " << m_port << "\n";
    }
    m_udpSocket.setBlocking(false);
    m_selector.add(m_udpSocket);



    while (true)
    {
        if (m_selector.wait())
        {
            // New connection
            if (m_selector.isReady(m_tcpListener))
            {
                auto* client = new sf::TcpSocket;
                if (m_tcpListener.accept(*client) == sf::Socket::Status::Done)
                {


                    UserConnected(*client);

                }
                else
                {
                    delete client;
                }
            }
            else
            {

                this->Handle();
                this->HandleUDP();
            }
        }

    }
}

MapJson& Server::GetMapJson()
{
    return m_mapJson;
}

void Server::AutoSaveLoop()
{
    while (true) {
        // Sleep for 5 seconds before performing the save
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // Perform the saving operation (for players, map, etc.)
        m_playersJson.SaveData();  // Adjust as needed for other data
        std::cout << "Auto-saving player data...\n";
    }
}

void Server::UserConnected(sf::TcpSocket& client)
{
    m_users.push_back(User(client));
    m_selector.add(client);
}

void Server::UserDisconnected(std::vector<User>::iterator& it)
{
    std::cout << "User disconnected: " << it->GetName() << "\n";
    m_selector.remove(*it->m_client);
    it = m_users.erase(it);
    this->PrintConnectedUsers();
}

void Server::SendMessageToClientTCP(User& user, const std::string& message)
{
    sf::Socket::Status status = user.m_client->send(message.c_str(), message.size());
    if (status == sf::Socket::Status::Done)
    {
        return;

    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        std::cerr << "Failed to send message to client: " << user.m_client->getRemoteAddress().value() << "\n";
        for (auto it = m_users.begin(); it != m_users.end(); ++it)
        {
            if(it->GetName() == user.GetName())
                this->UserDisconnected(it);
            
        }


    }
}

void Server::SendMessageToClientUDP(const sf::IpAddress& ip, unsigned short port, const std::string& message)
{
    sf::Socket::Status status = m_udpSocket.send(message.c_str(), message.size(), ip, port);

    if (status != sf::Socket::Status::Done)
    {
       std::cerr << "[UDP] Failed to send message to " << ip.toString() << ":" << port << "\n";
    }

}

void Server::PrintConnectedUsers() const
{
    std::cout << "Connected users (" << m_users.size() << "):\n";
    for (const auto& user : m_users)
    {
       
        user.PrintUser();
        //std::cout << "- " << user.GetName() << '\n';
        
    }
}


void Server::Handle()
{

    // Check all clients
    for (auto it = m_users.begin(); it != m_users.end();)
    {
		auto& user = *it;
        if (m_selector.isReady(*user.m_client))
        {
            char buffer[1024];
            std::size_t received;
            sf::Socket::Status status = user.m_client->receive(buffer, sizeof(buffer), received);
            if (status == sf::Socket::Status::Done)
            {

                std::string response(buffer, received);
                //std::cout << "Received: " << message << "\n";
                try {
                    json j = json::parse(response);  // Parse the received string into a JSON object
                    if (j.contains("action"))
                    {
                        std::cout << "[TCP] Received: " << j["action"] << "\n";


                        if (j["action"] == "InitUser")
                            this->InitUser(*it, j["data"]);
                        if (j["action"] == "InitPlayer")
                            this->InitPlayer(*it, j["data"]);
                        if (j["action"] == "SendPlayer")
                            this->SendPlayer(*it);
                        if (j["action"] == "SendMapData")
                            this->SendAllMapData(*it);
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "[TCP] Error parsing received message: " << e.what() << "\n";

                }

            }
            else if (status == sf::Socket::Status::Disconnected)
            {
                UserDisconnected(it);

                continue;
            }
            else if (status == sf::Socket::Status::Error)
            {
                std::cerr << "[TCP] Error receiving data from client: " << user.m_client->getRemoteAddress().value() << "\n";
                UserDisconnected(it);
                continue;
            }
        }
        ++it;
    }
}

void Server::HandleUDP()
{
    char buffer[1024];
    std::size_t received;
    std::optional<sf::IpAddress> sender; // Use std::optional to handle the sender address
    unsigned short senderPort;

    // Check if there is data available on the UDP socket
    sf::Socket::Status status = m_udpSocket.receive(buffer, sizeof(buffer), received, sender, senderPort);

    if (status == sf::Socket::Status::Done) {


        std::string message(buffer, received);
        try {
            json j = json::parse(message);


			for (auto it = m_users.begin(); it != m_users.end(); ++it)
			{
				if (it->GetName() == j["username"])
				{
				
                    if (j.contains("action"))
                    {


                       // std::cout << "[UDP] Received: " << j["action"]<< "from: " << it->GetName() << "\n";
                        if (j["action"] == "Ping") {
                            std::cout << "[UDP] Ping received from " << sender->toString() << '\n';
                        }
                        if (j["action"] == "UpdatePlayer") {
                            this->UpdatePlayer(*it, j["data"], senderPort);
                            break;
                        }
						if (j["action"] == "SendAllPlayers") {
							this->SendAllPlayers(*it, senderPort);
							break;
						}


                    }
				}
			}

            
        }


        catch (const std::exception& e) {
            std::cerr << "[UDP] Error parsing JSON: " << e.what() << '\n';
        }

    }
    else if (status == sf::Socket::Status::Disconnected) {
        std::cerr << "[UDP] Disconnected from sender: " << sender->toString() << '\n';
    }
    else if (status == sf::Socket::Status::Error) {
        std::cerr << "[UDP] Error receiving data\n";
    }
    else if (status == sf::Socket::Status::NotReady) {
        std::cerr << "[UDP] Socket not ready\n";
    }
}

  
     



void Server::InitUser(User& user, const std::string& message)
{
    user.InitUser(message);
    std::cout << "Server:User initialized: " << user.GetName() << "\n";
    PrintConnectedUsers();
}

void Server::InitPlayer(User& user, const json& player_data)
{

    if (!m_playersJson.CheckIfPlayerExists(user.GetName()))
    {
        m_playersJson.AddPlayer(user.GetName(), player_data);
        std::cout << "Server:Player initialized: " << m_playersJson.GetPlayer(user.GetName()) << "\n";
        
    }
    //SendMessageToClientTCP(user, "Player initialized from server");


    
}

void Server::SendPlayer(User& user)
{
    json player = {};
    player[user.GetName()] = m_playersJson.GetPlayer(user.GetName());
    std::string playerStr = player.dump();
    SendMessageToClientTCP(user, playerStr);
    //std::cout << "Server:Player sent: " << player["name"] << "\n";
}

void Server::UpdatePlayer(User& user, const json& player_data, unsigned short udp_port)
{
    m_playersJson.UpdatePlayer(user.GetName(), player_data);
    //json j{ "hello" };
    //SendMessageToClientUDP(user.m_client->getRemoteAddress().value(), udp_port, j);

}

void Server::SendAllPlayers(User& user, unsigned short udp_port)
{
	json allPlayers = {};
	for (auto& u : m_users)
	{
		if (u.GetName() == user.GetName())
			continue;

		allPlayers[u.GetName()] = m_playersJson.GetPlayer(u.GetName());
	}
	std::string allPlayersStr = allPlayers.dump();
	SendMessageToClientUDP(user.m_client->getRemoteAddress().value(), udp_port, allPlayersStr);
	//std::cout << "Server:All players sent to: " << user.GetName() << "\n";
}

void Server::SendAllMapData(User& user)  
{  
   try  
   {  
       std::string sendAllMapDataStr = m_mapJson.GetMapData(m_mapName).dump();  
       SendMessageToClientTCP(user, sendAllMapDataStr);  
       std::cout << "Server: Map data sent successfully to user: " << user.GetName() << "\n";  
   }  
   catch (const std::exception& e)  
   {  
       std::cerr << "Error sending map data: " << e.what() << "\n";  
   }  
}

