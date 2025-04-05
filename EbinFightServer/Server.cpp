#include "Server.h"
#include <iostream>

Server::Server(unsigned short port) : m_port(port)
{
}

Server::~Server()
{
    for (auto it = m_users.begin(); it != m_users.end();)
    {
        try {
            std::cout << "deleting: " + (*it)->GetName() << '\n';
            UserDisconnected(it);
        }
        catch (...) {
            std::cout << "failed deleting: " + (*it)->GetName() << '\n';
        }
    }
}


void Server::Listener()
{
    if (m_listener.listen(m_port) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to bind listener to port " << m_port << "\n";
        return;
    }

    m_selector.add(m_listener);
    std::cout << "Server is listening on port " << m_port << "\n";

    while (true)
    {
        if (m_selector.wait())
        {
            // New connection
            if (m_selector.isReady(m_listener))
            {
                auto* client = new sf::TcpSocket;
                if (m_listener.accept(*client) == sf::Socket::Status::Done)
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
            }
        }
    }
}

void Server::UserConnected(sf::TcpSocket& client)
{
    m_users.push_back(new User(client));
    std::cout << "New client connected: " << client.getRemoteAddress().value() << "\n";
    m_selector.add(client);
}

void Server::UserDisconnected(std::vector<User*>::iterator& it, sf::TcpSocket* client)
{
    std::cout << "User disconnected: " << (*it)->GetName()  << "\n";
    if(client)
        m_selector.remove(*client);
    delete* it;
    it = m_users.erase(it);
    this->PrintConnectedUsers();
}

void Server::SendMessageToClient(sf::TcpSocket& client, const std::string& message)
{
    sf::Socket::Status status = client.send(message.c_str(), message.size());
    if (status == sf::Socket::Status::Done)
    {
        return;

    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        std::cerr << "Failed to send message to client: " << client.getRemoteAddress().value() << "\n";
        for (auto it = m_users.begin(); it != m_users.end(); ++it)
        {
            if ((*it)->m_client == &client)
            {

                this->UserDisconnected(it, &client);
            }
        }


    }
}

void Server::PrintConnectedUsers() const
{
    std::cout << "Connected users (" << m_users.size() << "):\n";
    for (const auto& user : m_users)
    {
        if (user)
        {
            //user->PrintUser();
            std::cout << "- " << user->GetName() << '\n';
        }
    }
}


void Server::Handle()
{
   
    // Check all clients
    for (auto it = m_users.begin(); it != m_users.end();)
    {
        sf::TcpSocket& client = *(*it)->m_client;
        if (m_selector.isReady(client))
        {
            char buffer[1024];
            std::size_t received;
            sf::Socket::Status status = client.receive(buffer, sizeof(buffer), received);
            if (status == sf::Socket::Status::Done)
            {
                std::string message(buffer, received);
                //std::cout << "Received: " << message << "\n";
                auto parts = Package::Unpack(message);
                std::cout << "Received: " << parts["action"] << "\n";
                if (parts["action"] == "name")
                    this->InitUser(**it, parts["data"]);
            }
            else if (status == sf::Socket::Status::Disconnected)
            {
                UserDisconnected(it, &client);
                
                continue;
            }
        }
        ++it;
    }
}

void Server::InitUser(User& user, const std::string& message)
{
    user.SetName(message);
    PrintConnectedUsers();
}

