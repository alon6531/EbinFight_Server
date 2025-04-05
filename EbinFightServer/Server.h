#pragma once

#include "User.h"


class Server
{
public:
    Server(unsigned short port);
    ~Server();


    void Listener();
   

private:
    void UserConnected(sf::TcpSocket& client);
    void UserDisconnected(std::vector<User*>::iterator& it, sf::TcpSocket* client = NULL);
    void SendMessageToClient(sf::TcpSocket& client, const std::string& message);
    void PrintConnectedUsers() const;

    void Handle();

    void InitUser(User& user, const std::string& message);
   

private:
    sf::TcpListener m_listener;
    std::vector<User*> m_users;
    sf::SocketSelector m_selector;
    unsigned short m_port;
};