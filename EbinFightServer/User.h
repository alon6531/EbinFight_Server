#pragma once
#include "Globals.h"
#include "PlayersJson.h"

class User
{
public:
	User(sf::TcpSocket& client);
	~User();


	void InitUser(const std::string& name);

	const std::string& GetName() const;


	void PrintUser() const;

public:
	sf::TcpSocket* m_client;
private:
	std::string m_name;
	
	//class Entity entity;

};

