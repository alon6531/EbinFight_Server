#include "User.h"

User::User(sf::TcpSocket& client) : m_client(&client)
{
    m_name = "alon";
}

void User::SetName(const std::string& name)
{
    m_name = name;
}

const std::string& User::GetName() const
{
    return m_name;
}

void User::PrintUser() const
{
    std::cout << " - " << "username: " << m_name << "\n {" << "\n    "
        << "client info: "
        << m_client->getRemoteAddress().value().toString() << ":"
        << m_client->getRemotePort() << "\n }" << "\n\n";
}
