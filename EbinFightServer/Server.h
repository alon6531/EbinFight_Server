#pragma once

#include "User.h"
#include "MapJson.h"
#include <thread>

class Server
{
public:
    Server(unsigned short port, const std::string& map_name = "map1");
    ~Server() = default;

    void Start();
    void Listener();
   
    MapJson& GetMapJson();

   


private:
    void UserConnected(sf::TcpSocket& client);
    void UserDisconnected(std::vector<User>::iterator& it);
    void SendMessageToClientTCP(User& user, const std::string& message);
    void SendMessageToClientUDP(const sf::IpAddress& ip, unsigned short port, const std::string& message);
    void PrintConnectedUsers() const;
    void AutoSaveLoop();


    void Handle();
	void HandleUDP();

    void InitUser(User& user, const std::string& message);

	void InitPlayer(User& user, const json& player_data);
    void SendPlayer(User& user);
	void UpdatePlayer(User& user, const json& player_data, unsigned short udp_port);

    void SendAllPlayers(User& user, unsigned short udp_port);

    void SendAllMapData(User& user);
   

private:
    sf::TcpListener m_tcpListener;
    std::vector<User> m_users;
    sf::SocketSelector m_selector;
    unsigned short m_port;

    sf::UdpSocket m_udpSocket;

    MapJson m_mapJson;
    PlayersJson m_playersJson;
    std::string m_mapName;
};